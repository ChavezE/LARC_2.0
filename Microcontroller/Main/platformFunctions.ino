///////////////////
// Todo Funciona //
///////////////////

//Move plattaform In
void platIn()
{
  lcd.clear();
  writeLCD("PLAT IN");
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
  lcd.clear();
  writeLCD("PLAT OUT");
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
  lcd.clear();
  writeLCD("OPEN CLAW");
  sClaw.write(30);
}

//Close claw of the robot
void closeClaw()
{
  lcd.clear();
  writeLCD("CLOSE CLAW");
  sClaw.write(60);
}
