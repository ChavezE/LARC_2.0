
#include <Adafruit_BNO055.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

///////////////////////
//        PINS       //
///////////////////////

//------Motors--------//

//Motor Front Right
const byte pinMFRA = 29;
const byte pinMFRB = 31;
const byte pinPWMFR = 2;

//Motor Front Left
const byte pinMFLA = 27;
const byte pinMFLB = 33;
const byte pinPWMFL = 3;

//Motor Back Right
const byte pinMBRA = 35;
const byte pinMBRB = 41;
const byte pinPWMBR = 5;

//Motor Back Left
const byte pinMBLA = 37;
const byte pinMBLB = 39;
const byte pinPWMBL = 4;

//Encoder
const byte pinEncoder = 18;

//-------Sharps--------//

//Front Sharp
const byte pinSF = A3;

//Back Sharp
const byte pinSB = A2;

//Sharp Right Front
const byte pinSRF = A4;

//Sharp Right Center
//const byte pinSRC;

//Sharp Right Back
const byte pinSRB = A1;

//Sharp Left Front
const byte pinSLF = A5;

//Sharp Left Center
//const byte pinSLC;

//Sharp Left Back
const byte pinSLB = A0;

//Sharp Claw
const byte pinSC = A6;

//pins Sharps
byte pinSharp[9] = {A3, A4, A1, A2, A0, A5, A6};

//----LimitSwithces----//

//In Limit Switch
const byte pinLI = 43;

//Out Limit Switch
const byte pinLO = 45;

const byte pinLR = 6;

const byte pinLL = 7;

//-------Servos-------//

//Claw Servo
const byte pinServoC = 8;
Servo sClaw;

//Plattaform Servo
const byte pinServoP = 9;
Servo sPlattaform;

/////////////////////
//    Constants    //
/////////////////////

//Counts of the encoder for 1 cm --need update
const unsigned long constEncoder = 5500UL;

//Correction with P for turns
const long constPTurn = 40L;

//Correction with P
const long constPCorrect = 30L;

//Correction P in distance
const long constPDist = 110L;

//P correction Nestor
const double constPCorrectN = 0.1;

//Constants of motors when the robot is treated as a tank

//Velocity for motors when moving forward or backwards
const long velForward = 70L;

//Velocity for motor when turning
const long velTurn = 60L;

//Constants of the motors when the motor is treated as  a 4x4

//Cosntants of motors velocity
const int velLF = 67; //67
const int velLB = 68; //68

const int velRF = 70; //70
const int velRB = 80; //80

//Cosntants of motors velocity for going slow
const int velSlowLF = 48;
const int velSlowLB = 51;

const int velSlowRF = 42;
const int velSlowRB = 56;

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

//Counts of encoder
const int encoder30Cm = 1500;

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Angles of West, East, North, South
int iNorth = 0;
int iSouth = 0;
int iWest = 0;
int iEast = 0;

void setup()
{
  lcd.clear();
  writeLCD("Iniciando", 0, 0);
  //delay(1000);
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
  //lcd.clear();

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

  sClaw.attach(pinServoC);
  sPlattaform.attach(pinServoP);

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

  //Display the finish of the setup
  lcd.clear();
  writeLCD("START FENIX 2.0", 0, 0);

  //Stop plattaform for security
  sPlattaform.write(90);

  //Angle for north
  iNorth = getCompass();
  //Angle for East
  iEast = iNorth + 90;
  //Cheack if is not bigger than 360
  if(iEast > 360){
    iEast -= 360;
  }
  //Angle for east
  iSouth = iEast + 90;
  //Angle for south
  iWest = iSouth + 90;
}

void loop()
{
  communication();
  // delay(500);
  // writeLCD("   ", 13, 1);
  // writeLCD(String(getCompass()), 13, 1);
}
