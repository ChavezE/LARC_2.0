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
  bool bMilker = digitalRead(pinServo);
  bool bMilker2 = digitalRead(pinServoAux);
  
  sMilker2.write(90);
  sMilker.write(90);
  
  if(!bMilker)
  {
    sMilker.write(100);
    while(!bMilker)
    {
      bMilker = digitalRead(pinServo);
    }
    sMilker.write(90);
  }
  
  if(!bMilker2)
  {
    sMilker2.write(100);
    while(!bMilker2)
    {
      bMilker2 = digitalRead(pinServoAux);
    }
    sMilker2.write(90);
  }
}

void push()
{
  sMilker2.write(100);
  sMilker.write(100);
  delay(4000);
  sMilker2.write(90);
  sMilker.write(90);
}

//Milk the cow, close the upper part and try to milk three times
void Milk()
{
  //Move up part of milker once
  //Start moving both servos
  sMilker2.write(100);
  sMilker.write(100);
  //Keep moving the servos until it touch the limit switch
  while(digitalRead(pinLU) == 0);
  //Stop servos
  sMilker2.write(90);
  sMilker.write(90);
  //Start moving the servos again for security
  sMilker2.write(100);
  sMilker.write(100);
  //Wait 700 miliseconds for the principal servo to pass
  delay(700);
  //Stop servos
  sMilker2.write(90);
  sMilker.write(90);

  //Move down part of milker three times
  //Counter of how many times does the movment has benn made
  int iCont = 0;
  //Start moving the servo for milk
  sMilker.write(60);
  //While the complete move hasn't benn made 3 times
  while(iCont < 3)
  {
    //If the limit switch is unpressed
    if(digitalRead(pinLD) == 0)
    {
      //Add one to count
      iCont++;
      //Wait till the limit switch is pressed again
      while(digitalRead(pinLD) == 0);
    }
    //delay to secure the rebounce
    delay(50);
  }
  //Stop moving
  sMilker.write(90);
}

void loop() {
  getReady();
  Serial.println("Ready");
  sMilker2.write(150);
  sMilker.write(180);
  //Keep moving the servos until it touch the limit switch
  while(digitalRead(pinLU) == 0);
  delay(80);
  //Stop servos
  sMilker2.write(90);
  delay(100);
  sMilker.write(90);

  sMilker.write(70);
  int iCounter = 0;
  while(true)
  {
    if(digitalRead(pinLD) == 0)
    {
      iCounter++;
      delay(100);
      while(digitalRead(pinLD) == 0);
      delay(80);
    }
    if(iCounter >= 3)
    {
      break;
    }
  }
  sMilker.write(90);
  //sMilker.write(80);
  //while(digitalRead(pinLD) == 1);
  delay(1000);
}
