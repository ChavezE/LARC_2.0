void getInCow(bool bSlow) {
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
  int distanceToWall = 25;
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
  distRB = getDistance(pinSRB);
  distLB = getDistance(pinSLB);

  //If right leg was touched
  if (!bRight) {
    //Turn 90 degrees to the left
    turnNDegrees(-90);
    //Go backward to make sure the sharp of the back havent detect the leg
    backwardNCm(10, false);
    //Forward till back sharp detect the leg
    forward(LF, LB, RF, RB);
    do {
      distRB = getDistance(pinSRB);
    } while (distRB > distanceToWall);
    //Keep moving until the back sharp lose the leg
    while (distRB < 30) {
      distRB = getDistance(pinSRB);
    }
    //Stop
    brake();
    //Move forward a little bit to get away from the leg
    forwardNCm(6, bSlow);
    //Turn 90 degrees to the right
    turnNDegrees(90);
    //Restart the function
    getInCow(bSlow);
    //forwardNCm(10, false);
  }
  //If left leg was touched
  else if (!bLeft) {
    //Turn 90 degrees to the right
    turnNDegrees(90);
    //Go backward to make sure the sharp of the back havent detect the leg
    backwardNCm(10, false);
    //Go forward until the back sharp detect the leg
    forward(LF, LB, RF, RB);
    do {
      distLB = getDistance(pinSLB);
    } while (distLB > distanceToWall);
    //Keep moving until the back sharp lose the leg
    while (distLB < 30) {
      distLB = getDistance(pinSLB);
    }
    //Stop
    brake();
    //Move forward a little to get away from the leg
    forwardNCm(4, bSlow);
    //Turn 90 degrees to th eleft
    turnNDegrees(-90);
    //Start funciton again
    getInCow(bSlow);
    //forwardNCm(10, false);
  }
  //If no leg was touched
  else {
    //If right sharp detect the leg
    if ((distRF > distanceToWall || distLF > distanceToWall) && !(distRF > distanceToWall && distLF > distanceToWall))
    {
      if (distRF < distanceToWall) {
        forward(LF, LB, RF, RB);
        distLF = getDistance(pinSLF);
        while (bLeft == normalState && distLF > distanceToWall)
        {
          bLeft = digitalRead(pinLL);
          distLF = getDistance(pinSLF);
        }
        brake();
        backwardNCm(10, false);
        distRF = getDistance(pinSRF);
        while (distRF > distanceToWall)
        {
          turnNDegrees(iTurn);
          distRF = getDistance(pinSRF);
        }
        forwardNCm(10, false);
      }
      else {
        forward(LF, LB, RF, RB);
        distRF = getDistance(pinSRF);
        while (bRight == normalState && distRF > distanceToWall)
        {
          bRight = digitalRead(pinLR);
          distRF = getDistance(pinSRF);
        }
        brake();
        backwardNCm(10, false);
        distLF = getDistance(pinSLF);
        while (distLF > distanceToWall)
        {
          turnNDegrees(iTurn * -1);
          distLF = getDistance(pinSLF);
        }
        forwardNCm(10, false);
      }
      brake();
    }
  }
  //To be sure
  brake();
}
