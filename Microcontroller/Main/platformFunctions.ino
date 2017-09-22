//Move plattaform In
void platIn()
{
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("PLAT IN", 0, 0);
  //Check the inner limit switch of the robot
  while (digitalRead(pinLI) == 1)
  {
    //Start moving the plattaform
    sPlattaform.write(100);
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
  sPlattaform.write(100);
  
  delay(2500);
  
  platformStop();
}

//Move plattaform Out
void platOut()
{
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("PLAT OUT", 0, 0);
  //Check the inner limit switch of the robot
  while (digitalRead(pinLO) == 1)
  {
    //Start moving the plattaform
    sPlattaform.write(80);
  }
  
  platformStop();
}

void platformStartToOut() {
  sPlattaform.write(80);
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
  sClaw.write(45);
  delay(1400);
}

//Close claw of the robot
void closeClaw()
{
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("CLOSE CLAW", 0, 0);
  sClaw.write(70);
  delay(1400);
}
