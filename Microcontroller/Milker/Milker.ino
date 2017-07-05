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

void setup() {
  Serial.begin(9600);

  //Attach servos
  sMilker.attach(9);
  sMilker2.attach(10);

  //Stop servos
  sMilker.write(90);
  sMilker2.write(90);

  //Limit switches
  pinMode(6, INPUT);
  pinMode(7, INPUT);
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
  Milk();
}
