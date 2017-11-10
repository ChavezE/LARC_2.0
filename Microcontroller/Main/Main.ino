#include <Adafruit_BNO055.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#include <Logger.h>
#include <SerialLog.h>
#include <LCDLogger.h>
#include <LevelLogger.h>
#include <Estadisticas.h>


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

//-------Encoder--------//
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

const byte pinSFunel = A7;

//pins Sharps
byte pinSharp[9] = {A3, A4, A1, A2, A0, A5, A6};

//----LimitSwithces----//
const bool normalState = 1;

const byte pinLI = 48;

const byte pinLO = 36;

const byte pinLR = 40;

const byte pinLL = 30;

const byte pinLLB = 34;

const byte pinLRB = 32;

const byte pinLMD = 38;

//Para los limits de arriba y abajo del ordeñador los pins 36 y 38

// Limit down garra
const byte pinLCD = 44;

// Limit up garra
const byte pinLCU = 46;

//-------Servos-------//
//Close open claw servo
const byte pinServoC = 19;  //4

//In out Plattaform Servo
const byte pinServoP = 7;

//Up down claw/plat servo
const byte pinServoCUD = 4;

//Rotate claw servo
const byte pinServoR = 0; //5

//------Ultrasonic----//
// Ultrasonic of the Claw
const byte pinUSC_T = 14;
const byte pinUSC_E = 18;

/////////////////////
//    Constants    //
/////////////////////

//--------Corrections-------//
//P for turns (it is multiplied)
const int constPTurn = 3;

//P for forward and backward (it is multiplied)
const int constPCorrect = 19; // 1/0.06

//Const for encoder. Perfect at 60cm, 30cm fail by -1.5cm, 100cm by 2cm, 150cm by 3cm
const long encoder30Cm = 4300L; // TODO: Better it depends in the distance.

//------Velocities as a tank------//
//Velocity for motors when moving forward or backwards
const int velForward = 70;

//Velocity for motor when turning
const int velTurn = 60;

//-------Velocities as a 4x4------//
/**
 * <=30 ya no jala, no se mueven nada
 * 50= lento, pero se mueven, jala.
 * 100= caminata con prisa
 * 150= caminata rapida. Ya con un torque
 * 200= trotando. Ya el torque remarcable
 * 255= trotando rapido. Torque chidote
 */
//Normal velocity
const int velLF = 158; //158
const int velLB = 158; //158

const int velRF = 135; //120
const int velRB = 135; //120

//Slow velocity
const int velSlowLF = 115;
const int velSlowLB = 115;

const int velSlowRF = 98;
const int velSlowRB = 98;

//-----Velocities for turn----//
const int velTurnLF = 175;
const int velTurnLB = 140;

const int velTurnRF = 150;
const int velTurnRB = 165;

/////////////////////
//    Variables    //
/////////////////////

//-----RESTART----//
bool inARestart = false; // THIS VARIABLE SHOULD BE CHANGED IN THE FUNCTION restart

//--Terrines init Position---//
// TODO: Add in goGrabTerrine, because a restart, to set this variable.
// Note: For now, only leaveTerrine uses this. Nearly, also goGrabTerrine.
bool terrinesInitInRight = true; // THIS VARIABLE SHOULD BE CHANGED EACH ROUND FOR NOW

//------BNO-------//
Adafruit_BNO055 bno;

//------Servos------//
//Close open claw servo
Servo sClaw;

//In out Plattaform Servo
Servo sPlattaform;

//Up down claw/plat servo
Servo sCUD;

//Rotate claw servo
Servo sCT;

//------Ultrasonics----//
NewPing ultrasonicClaw(pinUSC_T, pinUSC_E, 200);

//------Encoder-------//
//Steps counted by the encoder
volatile unsigned long steps = 0;

//State of the encoder
//0->Stop   1->Forward    2->Backwards
volatile byte encoderState = 0;

//--------LCD-------//
LiquidCrystal_I2C lcd(0x27, 16, 2);


//--------DIRECTIONS------//
//Angles of West, East, North, South
int iNorth = 0;
int iSouth = 0;
int iWest = 0;
int iEast = 0;

///////////////////////
//  Methods headers  //
///////////////////////
void clawToStartPoint(bool safeAndSlow = false);
int getDistance(byte sharp, byte cantReads = 7);

//Communication with milker
const byte iMilkerA = 0;
const byte iMilkerB = 17;

//Milker up/down
const byte pinMotA = 15;
const byte pinMotB = 16;

//Pins of extractor
const byte pinExtractorA = 8;
const byte pinExtractorB = 6;

//I2C communication with nano
int const SLAVE_ID = 10;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  //Initialize lcd, turn backlight on and clear the display
  lcd.init();
  lcd.backlight();
  lcd.clear();

  //Check BNO
  if (!bno.begin(Adafruit_BNO055::OPERATION_MODE_NDOF))
  {
    Serial.println("NO BNO, while(1)");
    writeLCD("NO BNO", 0, 0);
    while(1);
  }
  bno.setExtCrystalUse(true);

  //Pin mode motors
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

  stopMilker();

  sClaw.attach(pinServoC);
  sPlattaform.attach(pinServoP);
  sCUD.attach(pinServoCUD);
  sCT.attach(pinServoR);

  //Pin mode limits
  pinMode(pinLI, INPUT);
  pinMode(pinLO, INPUT);
  pinMode(pinLCD, INPUT);
  pinMode(pinLCU, INPUT);

  pinMode(pinLL, INPUT);
  pinMode(pinLR, INPUT);
  pinMode(pinLLB, INPUT);
  pinMode(pinLRB, INPUT);
  pinMode(pinLMD, INPUT);

  //Add the encoder
  pinMode(pinEncoder, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);

  pinMode(iMilkerA, OUTPUT);
  pinMode(iMilkerB, OUTPUT);

  pinMode(pinExtractorA, OUTPUT);
  pinMode(pinExtractorB, OUTPUT);

  pinMode(pinMotA, OUTPUT);
  pinMode(pinMotB, OUTPUT);

  //Attach servos
  sClaw.attach(pinServoC);
  sPlattaform.attach(pinServoP);
  sCUD.attach(pinServoCUD);
  sCT.attach(pinServoR);

  //Stop motors, encoders and servos
  brake();

  encoderState = 1;

  openClaw();
  // clawToStartPoint();
  downClaw(); // This two for starting the first time ready to grab terrine
  platOut(); // THEN, IN THE FUNCTION restarting SHOULD BE A CALL TO clawToStartPoint()

  //Get angle for north, south, west and east
  //Angles if the robot start looking to the left
  // iWest = getCompass();
  // iNorth = iWest + 90;
  // if(iNorth > 360)
  // {
  //   iNorth -= 360;
  // }
  // iEast = iNorth + 90;
  // if(iEast > 360)
  // {
  //   iEast -= 360;
  // }
  // iSouth = iEast + 90;
  // if(iSouth > 360)
  // {
  //   iSouth -= 360;
  // }
  //Angles if the robot start looking at the cows
  iNorth = getCompass();
  iEast = iNorth + 90;
  if (iEast > 360) {
    iEast -= 360;
  }
  iSouth = iEast + 90;
  iWest = iSouth + 90;

  //Display the finish of the setup
  writeLCD("      El Mitosao", 0, 0);
  delay(1000);
  writeLCD("START El Mitosao", 0, 0);
}

void loop()
{

  communication();

}
