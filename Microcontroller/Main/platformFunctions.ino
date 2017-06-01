///////////////////
// Todo Funciona //
///////////////////

//Move plattaform In
void platIn()
{
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
  sClaw.write(30);
}

//Close claw of the robot
void closeClaw()
{
  sClaw.write(60);
}
