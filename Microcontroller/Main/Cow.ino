void getInCow(){
  //Clear LCD and write that we are getting under the cow
  lcd.clear();
  writeLCD("Entrando a vaca", 0, 0);

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

  //Reset vel
  LF = velSlowLF;
  LB = velSlowLB;
  RF = velSlowRF;
  RB = velSlowRB;

  //Check sharps
  distRB = getDistance(pinSRB);
  distLB = getDistance(pinSLB);

  if(bRight){
    turnNDegrees(-90);
    backwardNCm(10, false);
    forward(LF, LB, RF, RB);
    do{
      distRB = getDistance(pinSRB);
    }while(distRB > 30);

    while(distRB < 30){
      distRB = getDistance(pinSRB);
    }
    brake();
    forwardNCm(4, false);

    turnNDegrees(90);
    forwardNCm(10, false);
  }
  else if(bLeft){
    turnNDegrees(90);
    backwardNCm(10, false);
    forward(LF, LB, RF, RB);
    do{
      distLB = getDistance(pinSLB);
    }while(distLB > 30);

    while(distLB < 30){
      distLB = getDistance(pinSLB);
    }
    brake();
    forwardNCm(4, false);

    turnNDegrees(-90);
    forwardNCm(10, false);
  }
  else{
    forwardNCm(10, false);
  }
  //To be sure
  brake();
  /*
   *There could be an adjustment with the angle of the robot here
   */
}
