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
    
  }
   //If left leg was touched
  else if (!bLeft) {
    
  }
  //If no leg was touched
  else {
    
  }
  brake();
}

