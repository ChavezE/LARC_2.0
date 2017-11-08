//Limit touched in getInCow, 0 for none, 1 for right and 2 for left
byte limitTouched = 0;
//firstSahrp to detect the leg of the cow, 1 for right and 2 for left
byte firstLeg = 1;

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
    parkingLeft(bSlow, 20);
    getInCow(bSlow );
    limitTouched = 1;
  }
   //If left leg was touched
  else if (!bLeft) {
    parkingRight(bSlow, 20);
    getInCow(bSlow);
    limitTouched = 2;
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
      firstLeg = 1;
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
          parkingLeft(bSlow, 20);
          turnNDegrees(iTurn);
          getInCow(bSlow);
          break;
        }
        distLF = getDistance(pinSLF);
      }
    }
    else{
      firstLeg = 2;
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
          parkingRight(bSlow, 20);
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

void lookRight(int& iDistanceFunel)
{
  for(int iI = 0; iI < 5; iI++)
  {
    turnNDegrees(3);
    iDistanceFunel = getDistance(pinSFunel);
    if(iDistanceFunel < 17)
    {
      break;
    }
  }
}

void lookLeft(int& iDistanceFunel)
{
  for(int iI = 0; iI < 5; iI++)
  {
    turnNDegrees(-3);
    iDistanceFunel = getDistance(pinSFunel);
    if(iDistanceFunel < 17)
    {
      break;
    }
  }
}
void centerWithGlove()
{
  int iTry = 0;
  int iDistanceFunel = getDistance(pinSFunel);
  int iAm = getCompass();
  bool bContinue = true;
  while(bContinue)
  {
    if(limitTouched == 1)
    {
      if(!(iDistanceFunel < 17))
      {
        lookLeft(iDistanceFunel);
        turnToObjectiveN(iAm);
        lookRight(iDistanceFunel);
      }
    }
    else if(limitTouched == 2){
      if(!(iDistanceFunel < 17))
      {
        lookRight(iDistanceFunel);
        turnToObjectiveN(iAm);
        lookLeft(iDistanceFunel);
      }
    }
    else{
      if(firstLeg == 1)
      {
        if(!(iDistanceFunel < 17))
        {
          lookLeft(iDistanceFunel);
          turnToObjectiveN(iAm);
          lookRight(iDistanceFunel);
        }
      }
      else{
        if(!(iDistanceFunel < 17))
        {
          lookRight(iDistanceFunel);
          turnToObjectiveN(iAm);
          lookLeft(iDistanceFunel);
        }
      }
    }
    if(iDistanceFunel < 17)
    {
      forwardNCm(5, true);
      iDistanceFunel = getDistance(pinSFunel);
      if(iDistanceFunel < 10)
      {
        lcd.clear();
        writeLCD("Ordena", 0, 0);
        //simpleMilker();
        bContinue = false;
      }
      else{
        iTry++;
      }
    }
    else
    {
      iTry++;
    }
    if(iTry >= 3)
    {
      lcd.clear();
      writeLCD("Ordena", 0, 0);
      //simpleMilker();
      bContinue = false;
    }
    else
    {
        //goToStart
        turnToObjectiveN(iAm);
    }
  }
}
