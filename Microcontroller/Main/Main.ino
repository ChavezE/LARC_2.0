#include <Adafruit_BNO055.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

///////////////////////
//        PINS       //
///////////////////////

//------Motors--------//
//Motor Front Right
const byte pinMFRA = 24;
const byte pinMFRB = 26;
const byte pinPWMFR = 11;

//Motor Front Left
const byte pinMFLA = 22;
const byte pinMFLB = 28;
const byte pinPWMFL = 12;

//Motor Back Right
const byte pinMBRA = 31;
const byte pinMBRB = 37;
const byte pinPWMBR = 9;

//Motor Back Left
const byte pinMBLA = 35;
const byte pinMBLB = 33;
const byte pinPWMBL = 10;

//Encoder
const byte pinEncoder = 2;

//-------Sharps--------//

//Front Sharp
const byte pinSF = A4;

//Back Sharp
const byte pinSB = A13;

//Sharp Right Front
const byte pinSRF = A8;

//Sharp Right Center
//const byte pinSRC;

//Sharp Right Back
const byte pinSRB = A10; 

//Sharp Left Front
const byte pinSLF = A12;

//Sharp Left Center
//const byte pinSLC;

//Sharp Left Back
const byte pinSLB = A9;

//Sharp Claw
const byte pinSC = A11;

//pins Sharps
byte pinSharp[9] = {A3, A4, A1, A2, A0, A5, A6};

//----LimitSwithces----//

const bool normalState = 1;

const byte pinLI = 48;

const byte pinLO = 42;

const byte pinLR = 40;

const byte pinLL = 30;

const byte pinLLB = 34;

const byte pinLRB = 32;

//Para los limits de arriba y abajo del ordeñador los pins 36 y 38

// Limit down garra
const byte pinLCD = 44;

// Limit up garra
const byte pinLCU = 46;

//Ordeñador
const byte pinMotA = 4;
const byte pinMotB = 6;

//-------Servos-------//

//Claw Servo
const byte pinServoC = 19;  //4
Servo sClaw;

//Plattaform Servo
const byte pinServoP = 7;
Servo sPlattaform;

const byte pinServoCUD = 8;
Servo sCUD;

const byte pinServoR = 4; //5
Servo sCT;

//------Ultrasonic----//
const byte pinUSC_T = 14;
const byte pinUSC_E = 18;

NewPing ultrasonicClaw(pinUSC_T, pinUSC_E, 200);

/////////////////////
//    Constants    //
/////////////////////

//Counts of the encoder for 1 cm --need update
const unsigned long constEncoder = 5500UL;

//Correction with P for turns (it is multiplied)
const int constPTurn = 1;

//Correction with P
const long constPCorrect = 30L;

//Correction P in distance
const long constPDist = 110L;

//P correction Nestor
const double constPCorrectN = 0.06;

//Constants of motors when the robot is treated as a tank

//Velocity for motors when moving forward or backwards
const long velForward = 70L;

//Velocity for motor when turning
const long velTurn = 60L;

/**
 * Constants of the motors when the motor is treated as  a 4x4
 *
 * <=30 ya no jala, no se mueven nada
 * 50= lento, pero se mueven, jala.
 * 100= caminata con prisa
 * 150= caminata rapida. Ya con un torque
 * 200= trotando. Ya el torque remarcable
 * 255= trotando rapido. Torque chidote
 */
//Cosntants of motors velocity
const int velLF = 158; //158
const int velLB = 158; //158

const int velRF = 135; //120
const int velRB = 135; //120

//Constants of motors velocity for going slow
const int velSlowLF = 115;
const int velSlowLB = 115;

const int velSlowRF = 98;
const int velSlowRB = 98;

// Constants of motors for turn
const int velTurnLF = 115;
const int velTurnLB = 115;

const int velTurnRF = 98;
const int velTurnRB = 98;

/////////////////////
//    Variables    //
/////////////////////

//------BNO-------//
Adafruit_BNO055 bno;

//------Encoder-----//

//Steps counted by the encoder
volatile unsigned long steps = 0;

//State of the encoder
//0->Stop   1->Forward    2->Backwards
volatile byte encoderState = 0;

//Counts of encoder. Perfect at 60cm, 30cm fail by -1.5cm, 100cm by 2cm, 150cm by 3cm
const int encoder30Cm = 4300; // TODO: Better it depends in the distance.

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Angles of West, East, North, South
int iNorth = 0;
int iSouth = 0;
int iWest = 0;
int iEast = 0;

void setup()
{
  //Delay to establish connection with raspberry
  Serial.begin(9600);

  //Check BNO
  if (!bno.begin(Adafruit_BNO055::OPERATION_MODE_NDOF))
  {
    Serial.println("NO BNO");
  }
  bno.setExtCrystalUse(true);

  //Initialize lcd, turn backlight on and clear the display
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(pinMFRA , OUTPUT);
  pinMode(pinMFRB , OUTPUT);
  pinMode(pinPWMFR, OUTPUT);

  pinMode(pinMFLA , OUTPUT);
  pinMode(pinMFLB , OUTPUT);
  pinMode(pinPWMFL, OUTPUT);

  pinMode(pinMBRA , OUTPUT);
  pinMode(pinMBRB , OUTPUT);
  pinMode(pinPWMBR, OUTPUT);

  pinMode(pinMBLA, OUTPUT);
  pinMode(pinMBLB, OUTPUT);
  pinMode(pinPWMBL, OUTPUT);

  pinMode(pinMotA, OUTPUT);
  pinMode(pinMotB, OUTPUT);

  sClaw.attach(pinServoC);
  sPlattaform.attach(pinServoP);
  sCUD.attach(pinServoCUD);
  sCT.attach(pinServoR);

  pinMode(pinLI, INPUT);
  pinMode(pinLO, INPUT);
  pinMode(pinLL, INPUT);
  pinMode(pinLR, INPUT);

  pinMode(pinEncoder, INPUT_PULLUP);
  //attachInterrupt(pinEncoder, encoderStep, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);

  brake();
  platIn();
  openClaw();
  encoderState = 1;

  //Stop plattaform for security
  //horizontalClaw();
  //platIn();
  //downClaw();
  sPlattaform.write(90);
  sCUD.write(90);
  sCT.write(0);
  openClaw();

  //Angle for north
  iNorth = getCompass();
  //Angle for East
  iEast = iNorth + 90;
  //Cheack if is not bigger than 360
  if (iEast > 360) {
    iEast -= 360;
  }
  //Angle for east
  iSouth = iEast + 90;
  //Angle for south
  iWest = iSouth + 90;

  //Display the finish of the setup
  writeLCD("START FENIX 2.0", 0, 0); 
}

void loop()
{ 
  
 communication();

}
