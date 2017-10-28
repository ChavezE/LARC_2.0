//Move plattaform In
void platIn()
{
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("PLAT IN", 0, 0);
  //Check the inner limit switch of the robot
  while (digitalRead(pinLI) == normalState)
  {
    //Start moving the plattaform
    sPlattaform.write(97);
  }

  platformStop();
}

//Move plattaform to a mid distance
void platInMid()
{
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("PLAT IN MID", 0, 0);
  
  //Start moving the plattaform
  sPlattaform.write(97);
  
  delay(2500);
  
  platformStop();
}

//Move plattaform Out
void platOut()
{
  while (digitalRead(pinLO) == normalState)
  {
    //Start moving the plattaform
    sPlattaform.write(83);
  }
  
  platformStop();
}

void platformStartToOut() {
  sPlattaform.write(83);
}

void platformStop() {
  sPlattaform.write(90);
}

//Open Claw of the robot
void openClaw()
{
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("OPEN CLAW", 0, 0);
  sClaw.write(5);
  delay(1400);
}

//Close claw of the robot
void closeClaw()
{
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("CLOSE CLAW", 0, 0);
  sClaw.write(30);
  delay(1400);
}

void upClaw()
{
  while(digitalRead(pinLCU) == normalState)
  {
    sCUD.write(96);  
  }
  sCUD.write(90);
}

void downClaw()
{
  while(digitalRead(pinLCD) == normalState)
  {
    sCUD.write(85);  
  }
  sCUD.write(90);
}

void verticalClaw()
{
  sCT.write(90);
}

void horizontalClaw()
{
  sCT.write(175);
}

