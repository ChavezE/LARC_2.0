//Function to go to the terrines zone and grab a terrine
void goGrabTerrines(int iDeg)
{
  lcd.clear();
  writeLCD("GRAB TERRINE", 0, 0);
  //If the claw sharp detect something
  bool bTerrine = false;
  //Vel to move on function
  int LF = velSlowLF;
  int LB = velSlowLB;
  int RF = velSlowRF;
  int RB = velSlowRB;
  //Distance detected by sharp on claw
  int clawDistance = getDistance(pinSC);
  //Distance on front
  int frontDistance = getDistance(pinSF);
  //Start moving forward
  forward(LF, LB, RF, RB);
  //Keep moving forward until the claw or front sharp detect something or a limit switch is pushed
  while (digitalRead(pinLL) == 0 && digitalRead(pinLR) == 0 && clawDistance > 30)// && frontDistance > 30)
  {
    clawDistance = getDistance(pinSC);
    frontDistance = getDistance(pinSF);
    writeLCD("WHILE", 0, 0);
  }
  lcd.clear();
  //Stop moving
  brake();
  //If left limit switch is pushed
  if (digitalRead(pinLL) == 1)
  {
    //If there is a wall in front
    if (frontDistance < 30)
    {
      //Get away from the wall
      backwardNCm(5, false);
    }
    //If there is no wall in front
    else
    {
      //Go back
      backwardNCm(10, false);
      //Get back to original angle
      turnToObjectiveN(iDeg);
      //Turn 90 deg to the right
      turnNDegrees(90);
      //Move forward 5 cm to get away from the corner
      forwardNCm(5, false);
      //Turn back to original position
      turnNDegrees(-90);
      //Start function again
      goGrabTerrines(iDeg);
    }
  }
  //If right limit switch is pushed
  else if (digitalRead(pinLR) == 1)
  {
    //If there is a wall in front
    if (frontDistance < 30)
    {
      //Get away from the wall
      backwardNCm(5, false);
    }
    //If there is no wall in front
    else
    {
      //Get away from corner
      backwardNCm(10, false);
      //Go bak to original angle
      turnToObjectiveN(iDeg);
      //Turn left 90 degrees
      turnNDegrees(-90);
      //Go forward 5 cm to get wawy fromcorner
      forwardNCm(5, false);
      //Go back to original angle
      turnNDegrees(90);
      //Start function again
      goGrabTerrines(iDeg);
    }
  }
  //While it doesn't grab a terrine
  while (!bTerrine)
  {
    //Get distance from claw
    clawDistance = getDistance(pinSC);
    //open the claw
    openClaw();
    //if there is something in front of the claw
    if (clawDistance < 30)
    {
      //Move plattform out
      platOut();
      //Close the claw
      closeClaw();
      //Move plattform in mid distance
      platInMid();
      backwardNCm(20, false);
      //Turn 90 degrees to the right
      turnNDegrees(90);
      //If there is something in front of the claw it grab a terrine
      if (getDistance(pinSC) < 30)
      {
        //Terrine grabbed
        bTerrine = true;
      }
      //If there is nothing in front of the claw
      else
      {
        //Go bak to original position
        turnNDegrees(-90);
      }
    }
    //If there is nothing in front of the claw
    else
    {
      bool bToogle = false;
      //Move until the claw detect something
      while (clawDistance > 30)
      {
        if (!bToogle)
        {
          forwardNCm(4, true);
        }
        else
        {
          backwardNCm(4, true);
        }
        clawDistance = getDistance(pinSC);
        if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
        {
          bToogle = true;
        }
      }
    }
  }
}
