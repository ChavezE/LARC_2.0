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
const byte pinSF = A4;

//Back Sharp
const byte pinSB = A1;

//Sharp Right Front
const byte pinSRF = A3;

//Sharp Right Center
const byte pinSRC;

//Sharp Right Back
const byte pinSRB = A2;

//Sharp Left Front
const byte pinSLF = A5;

//Sharp Left Center
const byte pinSLC;

//Sharp Left Back
const byte pinSLB = A6;

//Sharp Claw
const byte pinSC = A0;

//----LimitSwithces----//

//In Limit Switch
const byte pinLI = 45;

//Out Limit Switch
const byte pinLO = 43;

//-------Servos-------//

//Claw Servo
const byte pinServoC = 6;
Servo sClaw;

//Plattaform Servo
const byte pinServoP = 7;
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

//Velocity for motors when moving forward or backwards
const long velForward = 70L;

//Velocity for motor when turning
const long velTurn = 60L;

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

//LCD
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  //Delay to establish connection with raspberry
  //delay(5000);
  Serial.begin(9600);

  //Check BNO
  if (!bno.begin(Adafruit_BNO055::OPERATION_MODE_NDOF))
  {
    //Serial.println("NO BNO");
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

  sClaw.attach(pinServoC);
  sPlattaform.attach(pinServoP);

  pinMode(pinLI, INPUT);
  pinMode(pinLO, INPUT);

  pinMode(pinEncoder, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);

  brake();
  //platIn();
  //openClaw();
  encoderState = 1;

  //Display the finish of the setup
  lcd.clear();
  writeLCD("START FENIX 2.0", 0, 0);
}

void loop()
{
  unsigned long data;
  unsigned long data1, data2;
  char order= '0';

  if(Serial.available() > 0){
    order = Serial.read();
    switch(order)
    {
      case 'a':
        //Serial.println("Brake Arduino");
        brake();
        Serial.write('1');
        break;

      case 'b':
        while(Serial.available() < 1);
        data1 = Serial.read();
        delay(1000);
        while(Serial.available() < 1);
        data2 = Serial.read();
        delay(1000);
        forward(data1, data2);
        Serial.write('1');
        break;

      case 'c':
        while(Serial.available() < 1);
        data1 = Serial.read();
        delay(1000);
        while(Serial.available() < 1);
        data2 = Serial.read();
        backward(data1, data2);
        Serial.write('1');
        break;

      case 'd':
        while(Serial.available() < 1);
        data = Serial.read();
        delay(1000);
        turnRight(data);
        Serial.write('1');
        break;

      case 'e':
        while(Serial.available() < 1);
        data = Serial.read();
        delay(1000);
        turnLeft(data);
        Serial.write('1');
        break;

      case 'f':
        while(Serial.available() < 1);
        data = Serial.read();
        delay(1000);
        turn(data);
        Serial.write('1');
        break;

      case 'g':
        while(Serial.available() < 1);
        data = Serial.read();
        delay(1000);
        forwardNCm(data);
        Serial.write('1');
        break;

      case 'h':
        while(Serial.available() < 1);
        data = Serial.read();
        delay(1000);
        backwardNCm(data);
        Serial.write('1');
        break;

      case 'i':
        while(Serial.available() < 1);
        data = Serial.read();
        delay(1000);
        forwardUntilWall(data);
        Serial.write('1');
        break;

      case 'j':
        while(Serial.available() < 1);
        data = Serial.read();
        delay(1000);
        backwardUntilWall(data);
        Serial.write('1');
        break;

      case 'k':
        while(Serial.available() < 1);
        data1 = Serial.read();
        delay(1000);
        while(Serial.available() < 1);
        data2 = Serial.read();
        delay(1000);
        turnToDegree(((data1<<8)|data2)*100UL);
        Serial.write('1');
        break;

      case 'l':
         while(Serial.available() < 1);
        data1 = Serial.read();
        delay(1000);
        while(Serial.available() < 1);
        data2 = Serial.read();
        delay(1000);
        turnRightNDegrees(((data1<<8)|data2)*100UL);
        Serial.write('1');
        break;

      case 'm':
        while(Serial.available() < 1);
        data = Serial.read();
        delay(1000);
        turnLeftNDegrees(data);
        Serial.write('1');
        break;

      case 'n':
        platIn();
        Serial.write('1');
        break;

      case 'o':
        platOut();
        Serial.write('1');
        break;

      case 'p':
        openClaw();
        Serial.write('1');
        break;

      case 'q':
        closeClaw();
        Serial.write('1');
        break;

      case 'r':
        Serial.println("Buscar distancia");
        Serial.print(getDistance(pinSF));
        break;

      case 's':
        Serial.write(getCompass());
        break;
    }
  }
}
