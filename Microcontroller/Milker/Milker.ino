//Servo library
#include <Servo.h>
#include <Wire.h>

//I2C communication const
int const SLAVE_ID = 10;
//Principal servo of milker
Servo sMilker;
//Auxiliar servo of milker
Servo sMilker2;

//Limit switch of upper part of milker
byte pinLU = 6;
//Limit switch of down part of milker
byte pinLD = 7;

//Servo Limits Pins
byte pinLServo = 5;
byte pinLServoAux = 4;

//Servo Pins
byte pinServo = 9;
byte pinServoAux = 10;

void setup() {
  Serial.begin(9600);

  //Attach servos
  sMilker.attach(pinServo);
  sMilker2.attach(pinServoAux);

  //Stop servos
  sMilker.write(90);
  sMilker2.write(90);

  //Limit switches
  pinMode(pinLU, INPUT);
  pinMode(pinLD, INPUT);
  pinMode(pinLServo, INPUT);
  pinMode(pinLServoAux, INPUT);
}

void getServoReady()
{
  //Limit switch of principal gear
  bool bMilker = digitalRead(pinLServo);

  //Move principal gear until the limit switch is activated
  if (!bMilker)
  {
    //Start moving
    sMilker.write(100);
    while (!bMilker)
    {
      //Read limit switch
      bMilker = digitalRead(pinLServo);
    }
    //Stop
    sMilker.write(90);
  }
}

void getServoAuxReady()
{
  //Limit switch in auxiliar gear
  bool bMilker2 = digitalRead(pinLServoAux);

  //Move auxiliar gear until the limit switch is activated
  if (!bMilker2)
  {
    //Start moving
    sMilker2.write(100);
    while (!bMilker2)
    {
      //Read limit switch
      bMilker2 = digitalRead(pinLServoAux);
    }
    //Stop
    sMilker2.write(90);
  }
}

//Move both servos to starting position
void getReady()
{
  //Stop servos
  sMilker2.write(90);
  sMilker.write(90);

  getServoReady();
  getServoAuxReady();
}

//Open upper part of the milker
void openMilker()
{
  //Separate the magnet
  sMilker2.write(180);
  sMilker.write(180);
  delay(200);
  //while (digitalRead(pinLServo) == 1 || digitalRead(pinLServoAux) == 1);
  //Keep moving the servos until it touch the limit switch
  while (digitalRead(pinLU) == 0);
  //Wait for the aux servo to get out
  //delay(80);
  //Stop out servo
  sMilker2.write(90);
  //wait for the principal servo to get out
  //delay(400);
  //Stop principal servo
  sMilker.write(90);
}

void closeMilker()
{
  sMilker2.write(140);
  sMilker.write(180);
  delay(80);
  //Stop out servo
  sMilker2.write(90);
  //wait for the principal servo to get out
  delay(400);
  //Stop principal servo
  sMilker.write(90);
}

void milk()
{
  //Timers
  unsigned long iStartTime;
  unsigned long iActTime;
  //Start milking
  sMilker.write(70);
  //Times the glove has been milked
  int iCounter = 0;
  while (iCounter < 8)
  {
    //If the milker is pressing the glove
    if (digitalRead(pinLD) == 0)
    {
      //Start time of the milker pressing the glove
      iStartTime = millis();
      //Wait until the milker get back to initial position
      while (digitalRead(pinLD) == 0);
      //Time when the milker stop pressing the glove
      iActTime = millis();
      //If the start and the finish time of the pressing process was bigger than .4 sec
      Serial.println(iActTime - iStartTime);
      if (iActTime - iStartTime >= 400)
      {
        //Times the pressing has been succesfull
        iCounter++;
      }
    }
  }
  //Stop milking process
  sMilker.write(90);
}

void milker()
{
  //Get servos in position
  getReady();
  //Open upper part of milker
  openMilker();
  //Time to get the glove inside the milker
  delay(4000);
  closeMilker();

  milk();
}

void tryLimits()
{
  Serial.print("pinLU: ");
  Serial.print(digitalRead(pinLU));
  Serial.print("    ");
  Serial.print("pinLD: ");
  Serial.print(digitalRead(pinLD));
  Serial.print("    ");
  Serial.print("pinLServo: ");
  Serial.print(digitalRead(pinLServo));
  Serial.print("    ");
  Serial.print("pinLServoAux: ");
  Serial.println(digitalRead(pinLServoAux));
}

void loop() {
  milker();
  delay(1000);
}
