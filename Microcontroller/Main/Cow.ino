void getInCow3(bool bRec, int iDeg)
{
  lcd.clear();

  if (bRec)
  {
    backwardNCm(5, false);
    turnToObjectiveN(iDeg);
  }
  else
  {
    //Angle at start
    int iAm = getCompass();
    //Velocity of motors
    int LF = velSlowLF;
    int LB = velSlowLB;
    int RF = velSlowRF;
    int RB = velSlowRB;

    //Degrees the robot will turn to try avoiding the legs when encountering one
    int iTurn = 6;
    int iLittle = 7;

    //Distances of sided front and back sharps
    int distRF = getDistance(pinSRF);
    int distLF = getDistance(pinSLF);
    int distRB = getDistance(pinSRB);
    int distLB = getDistance(pinSLB);

    //the leg that touch was the right one
    bool bRight = digitalRead(pinLR);
    //The leg that touch was the left one
    bool bLeft = digitalRead(pinLL);
    //Angle error
    int iError = 0;

    if (bRight == false && bLeft == false)
    {
      forward(LF, LB, RF, RB);
      while (digitalRead(pinLR) == false && digitalRead(pinLL) == false && distLF > 30 && distRF > 30)
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
        backwardNCm(10, true);
      }
    }
    if (bRight)
    {
      //Distance of right front sharp
      distRF = getDistance(pinSRF);

      lcd.clear();
      writeLCD("Buscando RF", 0, 0);
      //Turn until it detect a leg
      while (distRF > 30)
      {
        turnNDegrees(iTurn * -1);
        iError += iTurn;
        distRF = getDistance(pinSRF);
      }
      brake();
      writeLCD("Encontrada", 0, 1);
      if (iError / iTurn < iLittle)
      {
        lcd.clear();
        writeLCD("POCO", 0, 0);;
        forwardNCm(11, true);

        distLF = getDistance(pinSLF);

        while (distLF > 30)
        {
          turnNDegrees(iTurn);
          distLF = getDistance(pinSLF);
        }
      }
      else
      {
        //go to the center of the cow
        distRB = getDistance(pinSRB);
        bool toogle = false;
        lcd.clear();
        writeLCD("Centrandose", 0, 0);
        do
        {
          if (toogle == false)
          {
            forward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
            while (distRB > 30)
            {
              distRB = getDistance(pinSRB);
              if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
              {
                getInCow3(true, iDeg);
              }
            }
            toogle = true;
          }
          else
          {
            backward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
            while (distRB > 30)
            {
              distRB = getDistance(pinSRB);
            }
          }
          brake();
          delay(700);
          distRB = getDistance(pinSRB);
        } while (distRB > 30);
        backwardNCm(8, false);

        //Distance of right front sharp
        distRF = getDistance(pinSRF);
        lcd.clear();
        writeLCD("Buscando RF", 0, 0);
        //Turn until it detect a leg
        while (distRF > 30)
        {
          turnNDegrees(iTurn);
          distRF = getDistance(pinSRF);
        }
        //Distance of right front sharp
        distLF = getDistance(pinSLF);
        writeLCD("Buscando LF", 0, 1);
        //Turn until it detect a leg
        while (distLF > 30)
        {
          turnNDegrees(iTurn, velLF, velLB, 0, 0);
          distLF = getDistance(pinSLF);
        }
      }
    }
    else if (bLeft)
    {
      //Distance of right front sharp
      distLF = getDistance(pinSLF);

      lcd.clear();
      writeLCD("Buscando LF", 0, 0);
      //Turn until it detect a leg
      while (distLF > 30)
      {
        turnNDegrees(iTurn);
        iError += iTurn;
        distLF = getDistance(pinSLF);
      }
      brake();
      writeLCD("Encontrada", 0, 1);
      if (iError / iTurn < 11)
      {
        lcd.clear();
        writeLCD("POCO", 0, 0);;
        forwardNCm(11, true);

        distRF = getDistance(pinSRF);

        while (distRF > 30)
        {
          turnNDegrees(iTurn * -1);
          distLF = getDistance(pinSRF);
        }
      }
      else
      {
        //go to the center of the cow
        distRB = getDistance(pinSRB);
        bool toogle = false;
        lcd.clear();
        writeLCD("Centrandose", 0, 0);
        do
        {
          if (toogle == false)
          {
            forward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
            while (distRB > 30)
            {
              distRB = getDistance(pinSRB);
              if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
              {
                getInCow3(true, iDeg);
              }
            }
            toogle = true;
          }
          else
          {
            backward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
            while (distRB > 30)
            {
              distRB = getDistance(pinSRB);
            }
          }
          brake();
          delay(700);
          distRB = getDistance(pinSRB);
        } while (distRB > 30);
        backwardNCm(8, false);

        //Distance of right front sharp
        distLF = getDistance(pinSLF);
        lcd.clear();
        writeLCD("Buscando LF", 0, 0);
        //Turn until it detect a leg
        while (distLF > 30)
        {
          turnNDegrees(iTurn * -1);
          distLF = getDistance(pinSLF);
        }
        //Distance of right front sharp
        distLF = getDistance(pinSRF);
        writeLCD("Buscando RF", 0, 1);
        //Turn until it detect a leg
        while (distRF > 30)
        {
          turnNDegrees(iTurn, 0, 0, velRF, velRB);
          distRF = getDistance(pinSRF);
        }
      }
    }

    lcd.clear();
    writeLCD("Listo", 0, 0);
    delay(800);
    //Get away from cow
    backwardNCm(20, true);

    //Get distances
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
    //Update angle
    iAm = getCompass();
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
    if (getDistance(pinSLF) > 30)
    {
      backwardNCm(20, false);
      turnNDegrees(-5);
    }
    else if (getDistance(pinSRF) > 30)
    {
      backwardNCm(20, false);
      turnNDegrees(5);
    }

    //Get distances
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
    //Update angle
    iAm = getCompass();
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
  }
  brake();
}
