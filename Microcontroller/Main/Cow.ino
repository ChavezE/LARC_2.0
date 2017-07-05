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

  //Degrees the robot will turn to try avoiding the legs when encountering one
  int iTurn = 6;

  //Distances of sided front and back sharps
  int distRF = getDistance(pinSRF);
  int distLF = getDistance(pinSLF);
  int distRB = getDistance(pinSRB);
  int distLB = getDistance(pinSLB);

  //the leg that touch was the right one
  bool bRight = digitalRead(pinLR);
  //The leg that touch was the left one
  bool bLeft = digitalRead(pinLL);

  forward(LF, LB, RF, RB);
  while(digitalRead(pinLR) == false && digitalRead(pinLL) == false && distLF > 30 && distRF > 30)
  {
    //P Correction
    forwardP(iAm, LF, LB, RF, RB, true);
    //Sharps
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
  }

  //the leg that touch was the right one
  bRight = digitalRead(pinLR);
  //The leg that touch was the left one
  bLeft = digitalRead(pinLL);
  brake();

  //Get away form the leg
  if (bRight == true || bLeft == true)
  {
    backwardNCm(5, true);
  }

  if(bRight)
  {
    //Distance of right front sharp
    distRF = getDistance(pinSRF);

    lcd.clear();
    writeLCD("Buscando RF", 0, 0);
    //Turn until it detect a leg
    while (distRF > 30)
    {
      turnNDegrees(iTurn * -1);
      distRF = getDistance(pinSRF);
    }
    brake();

    //Turn until it lost leg
    while (distRF < 30)
    {
      turnNDegrees(iTurn * -1);
      distRF = getDistance(pinSRF);
    }
    brake();

    //Turn until detect leg
    while (distRF < 30)
    {
      turnNDegrees(iTurn);
      distRF = getDistance(pinSRF);
    }
    brake();

    //Distance of back sharp
    distRB = getDistance(pinSRB);
    //Move forward until back sharp detect the leg
    forward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
    while(distRB > 30)
    {
      distRB = getDistance(pinSRB);
    }
    brake();
    /*
    //Move forward until back sharp lose the leg
    forward(LF, LB, RF, RB);
    while(distRB < 30)
    {
      distRB = getDistance(pinSRB);
    }
    brake();
    */

    backwardNCm(7, true);
    //left front and right front sharp distance
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
    //Turn until a leg detect a leg
    while(distRF > 30 || distLF > 30)
    {
      turnNDegrees(iTurn);
      distRF = getDistance(pinSRF);
      distLF = getDistance(pinSLF);
    }
    forwardNCm(5, true);
  }
  else if(bLeft)
  {
    //Distance of right front sharp
    distLF = getDistance(pinSLF);

    lcd.clear();
    writeLCD("Buscando LF", 0, 0);
    //Turn until it detect a leg
    while (distLF > 30)
    {
      turnNDegrees(iTurn);
      distLF = getDistance(pinSLF);
    }
    brake();

    //Turn until it lost leg
    while (distLF < 30)
    {
      turnNDegrees(iTurn);
      distLF = getDistance(pinSLF);
    }
    brake();

    //Turn until detect leg
    while (distLF < 30)
    {
      turnNDegrees(iTurn * -1);
      distLF = getDistance(pinSLF);
    }
    brake();

    //Distance of back sharp
    distLB = getDistance(pinSLB);
    //Move forward until back sharp detect the leg
    forward(LF, LB, RF, RB);
    while(distLB > 30)
    {
      distLB = getDistance(pinSLB);
    }
    brake();
    //Move forward until back sharp lose the leg
    forward(LF, LB, RF, RB);
    while(distLB < 30)
    {
      distLB = getDistance(pinSLB);
    }
    brake();
    //left front and right front sharp distance
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
    //Turn until a leg detect a leg
    while(distRF > 30 || distLF > 30)
    {
      turnNDegrees(iTurn * -1);
      distRF = getDistance(pinSRF);
      distLF = getDistance(pinSLF);
    }
  }
  
  //Turn with a pivot
  if(distRF < 30)
  {
    while(distLF > 30)
    {
      turnNDegrees(iTurn, LF, LB, 0, 0);
      distLF = getDistance(pinSLF);
    }
  }
  else
  {
    while(distRF > 30)
    {
      turnNDegrees(iTurn * -1, 0, 0, RF, RB);
      distRF = getDistance(pinSRF);
    }
  }
  //Get in cow
  forwardNCm(5, true);
}
