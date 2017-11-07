void getInCow(bool bSlow)
{
  //Clear LCD and write that we are getting under the cow
  lcd.clear();
  writeLCD("Entrando a vaca", 0, 0);

  //Angle at start
  int iAm = getCompass();

  //Velocity of motors
  int LF;
  int LB;
  int RF;
  int RB;

  if (bSlow)
  {
    LF = velSlowLF;
    LB = velSlowLB;
    RF = velSlowRF;
    RB = velSlowRB;
  }
  else
  {
    LF = velLF;
    LB = velLB;
    RF = velRF;
    RB = velRB;
  }

  //Degrees the robot will turn to try avoiding the legs when encountering one
  int iTurn = 9;

  //Distances of sided front and back sharps
  int distRF = getDistance(pinSRF);
  int distLF = getDistance(pinSLF);
  int distRB = getDistance(pinSRB);
  int distLB = getDistance(pinSLB);

  //the leg that touch was the right one
  bool bRight = digitalRead(pinLR);
  //The leg that touch was the left one
  bool bLeft = digitalRead(pinLL);
  int distanceToWall = 20;
  forward(LF, LB, RF, RB);
  while (digitalRead(pinLR) == normalState && digitalRead(pinLL) == normalState && distLF > distanceToWall && distRF > distanceToWall)
  {
    //P Correction
    forwardP(iAm, LF, LB, RF, RB, bSlow);
    //Sharps
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
  }
  lcd.clear();
  //the leg that touch was the right one
  bRight = digitalRead(pinLR);
  //The leg that touch was the left one
  bLeft = digitalRead(pinLL);
  brake();

  //Get away form the leg
  if (bRight == !normalState || bLeft == !normalState)
  {
    backwardNCm(5, true);
  }

  //Reset vel
  if (bSlow)
  {
    LF = velSlowLF;
    LB = velSlowLB;
    RF = velSlowRF;
    RB = velSlowRB;
  }
  else
  {
    LF = velLF;
    LB = velLB;
    RF = velRF;
    RB = velRB;
  }

  //Check sharps
  // distRB = getDistance(pinSRB);
  // distLB = getDistance(pinSLB);

   //If right leg was touched
  if (!bRight) {
    parkingLeft(bSlow, 10);
    getInCow(bSlow );
  }
   //If left leg was touched
  else if (!bLeft) {
    parkingRight(bSlow, 10);
    getInCow(bSlow);
  }
  //If no leg was touched
  else {
    brake();
    // unsigned long start = millis();
    // unsigned long actual = millis();
    // int iWatchDog = 2000;
    int iStepsWatchDog = (encoder30Cm / 30) * 20;
    if(distRF < distanceToWall)
    {
      backwardNCm(5, bSlow);
      distLF = getDistance(pinSLF);
      steps = 0;
      forward(LF, LB, RF, RB);
      while(distLF > distanceToWall)
      {
        // actual = millis();
        // if(actual - start > iWatchDog)
        if(steps > iStepsWatchDog)
        {
          brake();
          backwardNCm(5, bSlow);
          parkingLeft(bSlow, 10);
          turnNDegrees(iTurn);
          getInCow(bSlow);
          break;
        }
        distLF = getDistance(pinSLF);
      }
    }
    else{
      backwardNCm(5, bSlow);
      distRF = getDistance(pinSRF);
      steps = 0;
      forward(LF, LB, RF, RB);
      while(distRF > distanceToWall)
      {
        // actual = millis();
        // if(actual - start > iWatchDog)
        if(steps > iStepsWatchDog)
        {
          brake();
          backwardNCm(5, bSlow);
          parkingRight(bSlow, 10);
          turnNDegrees(iTurn * -1);
          getInCow(bSlow);
          break;
        }
        distRF = getDistance(pinSRF);
      }
    }
  }
  brake();
  //forwardNCm(5, false);
}
