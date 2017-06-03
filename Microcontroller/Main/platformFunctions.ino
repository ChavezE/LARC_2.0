///////////////////
// Todo Funciona //
///////////////////

//Move plattaform In
void platIn()
{
  //Display function in LCD
  lcd.clear();
  writeLCD("PLAT IN", 0, 0);
  //Check the inner limit switch of the robot
  while (digitalRead(pinLI) == 0)
  {
    //Start moving the plattaform
    sPlattaform.write(100);
  }
  //Stop the plattaform
  sPlattaform.write(90);
}

//Move plattaform Out
void platOut()
{
  //Display function in LCD
  lcd.clear();
  writeLCD("PLAT OUT", 0, 0);
  //Check the inner limit switch of the robot
  while (digitalRead(pinLO) == 0)
  {
    //Start moving the plattaform
    sPlattaform.write(80);
  }
  //Stop the plattaform
  sPlattaform.write(90);
}

//Open Claw of the robot
void openClaw()
{
  //Display function in LCD
  lcd.clear();
  writeLCD("OPEN CLAW", 0, 0);
  sClaw.write(30);
}

//Close claw of the robot
void closeClaw()
{
  //Display function in LCD
  lcd.clear();
  writeLCD("CLOSE CLAW", 0, 0);
  sClaw.write(60);
}
