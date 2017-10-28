//Servo library
#include <Servo.h>

//Principal servo of milker
Servo sMilker;
//Auxiliar servo of milker
Servo sMilker2;

//Limit switch of upper part of milker
byte pinLU = 6;
//Limit switch of down part of milker
byte pinLD = 7;

//Servo Pins
byte pinServo = 5;
byte pinServoAux = 4;

void setup() {
  Serial.begin(9600);

  //Attach servos
  sMilker.attach(9);
  sMilker2.attach(10);

  //Stop servos
  sMilker.write(90);
  sMilker2.write(90);

  //Limit switches
  pinMode(pinLU, INPUT);
  pinMode(pinLD, INPUT);
  pinMode(pinServo, INPUT);
  pinMode(pinServoAux, INPUT);
}

void getReady()
{
  //Limit switch of principal gear
  bool bMilker = digitalRead(pinServo);
  //Limit switch in auxiliar gear
  bool bMilker2 = digitalRead(pinServoAux);

  //Stop servos
  sMilker2.write(90);
  sMilker.write(90);

  //Move principal gear until the limit switch is activated
  if (!bMilker)
  {
    //Start moving
    sMilker.write(100);
    while (!bMilker)
    {
      //Read limit switch
      bMilker = digitalRead(pinServo);
    }
    //Stop
    sMilker.write(90);
  }
  //Move auxiliar gear until the limit switch is activated
  if (!bMilker2)
  {
    //Start moving
    sMilker2.write(100);
    while (!bMilker2)
    {
      //Read limit switch
      bMilker2 = digitalRead(pinServoAux);
    }
    //Stop
    sMilker2.write(90);
  }
}

void openMilker()
{
  //Timers
  unsigned long iStartTime;
  unsigned long iActTime;
  //Separate the magnet
  sMilker2.write(140);
  sMilker.write(180);
  iStartTime = millis();
  //Keep moving the servos until it touch the limit switch
  while (digitalRead(pinLU) == 0)
  {
    //Check time of action
    iActTime = millis();
    //If the limit wasn't touched after 0.8 sec start over
    if (iActTime - iStartTime >= 800)
    {
      sMilker2.write(90);
      sMilker.write(90);
      getReady();
      sMilker2.write(140);
      sMilker.write(180);
      iStartTime = millis();
    }
  }
  //Wait for the aux servo to get out
  delay(80);
  //Stop out servo
  sMilker2.write(90);
  //wait for the principal servo to get out
  delay(100);
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
  while (iCounter < 3)
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
  //Milk the glove
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
  Serial.print("pinServo: ");
  Serial.print(digitalRead(pinServo));
  Serial.print("    ");
  Serial.print("pinServoAux: ");
  Serial.println(digitalRead(pinServoAux));
}

void loop() {
  //tryLimits();
  milker();
  delay(1000);
}
