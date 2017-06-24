//Get inside the cow
void getInCow()
{
  lcd.clear();
  //Angle at start
  int iAm = getCompass();
  //Velocity of motors
  int LF = velSlowLF;
  int LB = velSlowLB;
  int RF = velSlowRF;
  int RB = velSlowRB;
  //Error of angle
  int iError = 0;
  //Degrees the robot will turn to try avoiding the legs when encountering one
  int iTurn = 4;
  //Distances of sided front and back sharps
  int distRF = getDistance(pinSRF);
  int distLF = getDistance(pinSLF);
  int distRB = getDistance(pinSRB);
  int distLB = getDistance(pinSLB);

  //Start moving
  forward(LF, LB, RF, RB);
  //Forward until it touch or detect a leg
  while (digitalRead(pinLL) == 0 && digitalRead(pinLR) == 0 && distLF > 30 && distRF > 30)
  {
    //P Correction
    forwardP(iAm, LF, LB, RF, RB, true);
    //Check distance
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
  }
  brake();
  //Show distances
  writeLCD(String(distRF), 0, 0);
  writeLCD(String(distLF), 0, 1);

  //If right limit touch
  if (digitalRead(pinLR) == 1)
  {
    //Get away form the leg
    backwardNCm(10, true);

    //Distance of right front sharp
    distRF = getDistance(pinSRF);

    //Turn until it detect a leg
    while (distRF > 30)
    {
      turnNDegrees(iTurn * -1);
      distRF = getDistance(pinSRF);
    }
    brake();
    //Show distance
    lcd.clear();
    writeLCD(String(distRF), 0, 0);
    //Turn until it lost the leg
    while (distRF < 30)
    {
      turnNDegrees(iTurn * -1);
      distRF = getDistance(pinSRF);
    }
    brake();
    //Show distance
    lcd.clear();
    writeLCD(String(distRF), 0, 0);

    //Turn until back sharo detect the leg
    distRB = getDistance(pinSRB);
    while (distRB > 30)
    {
      turnNDegrees(iTurn * -1);
      distRB = getDistance(pinSRB);
    }

    //go to the center of the cow
    forwardNCm(10, true);

    //Correct the angle
    turnToObjectiveN(iAm);
    //Get away from cow
    backwardNCm(20, true);

    //Get distances
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
    //Start moving
    forward(LF, LB, RF, RB);
    //Forward until it detect a leg
    while (distLF > 30 && distRF > 30)
    {
      //P Correction
      forwardP(iAm, LF, LB, RF, RB, true);
      //Check distance
      distRF = getDistance(pinSRF);
      distLF = getDistance(pinSLF);
    }
    brake();
    //Show distances
    lcd.clear();
    writeLCD(String(distRF), 0, 0);
    writeLCD(String(distLF), 0, 1);
  }
  //If left limit touch
  else if (digitalRead(pinLL) == 1)
  {
    //Get away form the leg
    backwardNCm(10, true);

    //Distance of left front sharp
    distLF = getDistance(pinSLF);

    //turn until it detect a leg
    while (distLF > 30)
    {
      turnNDegrees(iTurn);
      distLF = getDistance(pinSLF);
    }
    brake();
    //Show distance
    lcd.clear();
    writeLCD(String(distLF), 0, 0);
    //Turn until it lost the leg
    while (distLF < 30)
    {
      turnNDegrees(iTurn);
      distLF = getDistance(pinSLF);
    }
    brake();
    //Show distance
    lcd.clear();
    writeLCD(String(distLF), 0, 0);
     //Forward until the back sharp detect the leg
    distLB = getDistance(pinSLB);
    forward(LF, LB, RF, RB);
    while(distLB > 30)
    {
      distLB = getDistance(pinSLB);
    }
    brake();

    //Correct the angle
    turnToObjectiveN(iAm);
    //Get away from cow
    backwardNCm(20, true);

    //Get distances
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
    //Start moving
    forward(LF, LB, RF, RB);
    //Forward until it detect a leg
    while (distLF > 30 && distRF > 30)
    {
      //P Correction
      forwardP(iAm, LF, LB, RF, RB, true);
      //Check distance
      distRF = getDistance(pinSRF);
      distLF = getDistance(pinSLF);
    }
    brake();
    //Show distances
    lcd.clear();
    writeLCD(String(distRF), 0, 0);
    writeLCD(String(distLF), 0, 1);
  }
}

//Get inside the cow
void getInCow2()
{
  lcd.clear();
  //Angle at start
  int iAm = getCompass();
  //Velocity of motors
  int LF = velSlowLF;
  int LB = velSlowLB;
  int RF = velSlowRF;
  int RB = velSlowRB;
  //Error of angle
  int iError = 0;
  //Degrees the robot will turn to try avoiding the legs when encountering one
  int iTurn = 4;
  //Funciton !ready
  bool bReady = false;
  //Distances of sided front sharps
  int distRF = getDistance(pinSRF);
  int distLF = getDistance(pinSLF);

  //Start moving
  forward(LF, LB, RF, RB);
  //Forward until it touch a leg
  while (digitalRead(pinLL) == 0 && digitalRead(pinLR) == 0 && distLF > 30 && distRF > 30)
  {
    //P Correction
    forwardP(iAm, LF, LB, RF, RB, true);
    //Check distance
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
  }
  writeLCD(String(distRF), 0, 0);
  writeLCD(String(distLF), 0, 1);
  brake();
  //If right limit touch
  if (digitalRead(pinLR) == 1)
  {
    //Get away form the leg
    backwardNCm(10, true);

    //Distance of right front sharp
    distRF = getDistance(pinSRF);

    //Forward until it touch a leg
    while (distRF > 30)
    {
      turnNDegrees(iTurn * -1);
      distRF = getDistance(pinSRF);
    }
    brake();
    //Show distance
    lcd.clear();
    writeLCD(String(distRF), 0, 0);
    while (distRF < 30)
    {
      turnNDegrees(iTurn * -1);
      distRF = getDistance(pinSRF);
    }
    brake();
    //Show distance
    lcd.clear();
    writeLCD(String(distRF), 0, 0);
    //Go to the center of the cow
    forwardNCm(20, true);
    //Correct the angle
    turnToObjectiveN(iAm);
    //Get away from cow
    backward(LF, LB, RF, RB);
    delay(800);
    brake();

    //Get distances
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
    //Start moving
    forward(LF, LB, RF, RB);
    //Forward until it detect a leg
    while (distLF > 30 && distRF > 30)
    {
      //P Correction
      forwardP(iAm, LF, LB, RF, RB, true);
      //Check distance
      distRF = getDistance(pinSRF);
      distLF = getDistance(pinSLF);
    }
    brake();
    //Show distances
    lcd.clear();
    writeLCD(String(distRF), 0, 0);
    writeLCD(String(distLF), 0, 1);
  }
  //If left limit touch
  else if (digitalRead(pinLL) == 1)
  {
    backward(LF, LB, RF, RB);
    delay(400);
    brake();

    //Distance of left front sharp
    distLF = getDistance(pinSLF);

    //Forward until it touch a leg
    while (distLF > 30)
    {
      turnNDegrees(iTurn);
      distLF = getDistance(pinSLF);
    }
    brake();
    //Show distance
    lcd.clear();
    writeLCD(String(distLF), 0, 0);
    while (distLF < 30)
    {
      turnNDegrees(iTurn);
      distLF = getDistance(pinSLF);
    }
    brake();
    //Show distance
    lcd.clear();
    writeLCD(String(distLF), 0, 0);
    forward(LF, LB, RF, RB);
    delay(600);
    brake();
    //Correct angle
    turnToObjectiveN(iAm);

    //Get away from cow
    backward(LF, LB, RF, RB);
    delay(800);
    brake();

    //Get distances
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
    //Start moving
    forward(LF, LB, RF, RB);
    //Forward until it detect a leg
    while (distLF > 30 && distRF > 30)
    {
      //P Correction
      forwardP(iAm, LF, LB, RF, RB, true);
      //Check distance
      distRF = getDistance(pinSRF);
      distLF = getDistance(pinSLF);
    }
    brake();
    //Show distances
    lcd.clear();
    writeLCD(String(distRF), 0, 0);
    writeLCD(String(distLF), 0, 1);
  }
}

void sharpTurn()
{
  lcd.clear();
  int distRF = getDistance(pinSRF);
  int distLF = getDistance(pinSLF);
  //Forward until it touch a leg
  while (distRF > 30 && distLF > 30)
  {
    turnNDegrees(5);
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
  }
  brake();
  writeLCD(String(distRF), 0, 0);
  writeLCD(String(distLF), 0, 1);
  while (distRF < 30 || distLF < 30)
  {
    turnNDegrees(5);
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
  }
  brake();
  writeLCD(String(distRF), 0, 0);
  writeLCD(String(distLF), 0, 1);
}

