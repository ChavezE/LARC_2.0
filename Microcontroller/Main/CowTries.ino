/*
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
  //the leg that touch was the right one
  bool bRight = false;

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
    bRight = true;
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
    //turnToObjectiveN(iAm);
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
    while (distLB > 30)
    {
      distLB = getDistance(pinSLB);
    }
    brake();

    //Correct angle
    //turnToObjectiveN(iAm);
  }

  //Angle correction
  if (bRight == true)
  {
    writeLCD("H", 5, 0);
    while (distRF > 30)
    {
      turnNDegrees(iTurn);
      distRF = getDistance(pinSRF);
    }
    writeLCD(String(getDistance(pinSRF)), 0, 0);
  }
  else {
    while (distLF > 30)
    {
      turnNDegrees(iTurn * -1);
      distLF = getDistance(pinSLF);
    }
    writeLCD(String(getDistance(pinSLF)), 0, 1);
  }

  //Correct the angle
  if (bRight == true)
  {
    while (distLF > 30)
    {
      turnNDegrees(iTurn, velSlowLF, velSlowLB, 0, 0);
      distLF = getDistance(pinSLF);
    }
    writeLCD(String(getDistance(pinSLF)), 0, 1);
  }
  else {
    while (distRF > 30)
    {
      turnNDegrees(iTurn * -1, 0, 0, velSlowRF, velSlowRB);
      distRF = getDistance(pinSRF);
    }
    writeLCD(String(getDistance(pinSRF)), 0, 0);
  }

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
  //Show distances
  //lcd.clear();
  //writeLCD(String(distRF), 0, 0);
  //writeLCD(String(distLF), 0, 1);
}

//Get inside the cow
void getInCow2()
{
  brake();
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
  //the leg that touch was the right one
  bool bRight = false;
  //The leg that touch was the left one
  bool bLeft = false;

  if (!(digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1))
  {
    writeLCD("Buscando Vaca", 0, 0);
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

    //If right limit touch
    if (digitalRead(pinLR) == 1)
    {
      writeLCD("Encontramos LR", 1, 0);
      bRight = true;
      //Get away form the leg
      backwardNCm(10, true);

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
      writeLCD("Encontrada", 0, 1);
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
              getInCow2();
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
      writeLCD("Listo", 0, 1);
      brake();
    }
    //If left limit touch
    else if (digitalRead(pinLL) == 1)
    {
      writeLCD("Encontramos LL", 1, 0);
      bLeft = true;
      //Get away form the leg
      backwardNCm(10, true);

      //Distance of left front sharp
      distLF = getDistance(pinSLF);

      lcd.clear();
      writeLCD("Buscando LF", 0, 0);
      //turn until it detect a leg
      while (distLF > 30)
      {
        turnNDegrees(iTurn);
        distLF = getDistance(pinSLF);
      }
      brake();
      writeLCD("Encontrada", 0, 1);

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
          while (distLB > 30)
          {
            distLB = getDistance(pinSLB);
          }
          toogle = true;
        }
        else
        {
          backward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
          while (distLB > 30)
          {
            distLB = getDistance(pinSLB);
          }
        }
        brake();
        delay(700);
        distLB = getDistance(pinSLB);
      } while (distLB > 30);
      backwardNCm(8, false);
      writeLCD("Listo", 0, 1);
      brake();
    }
  }

  //Check if at start a lmit is touching a leg
  if (digitalRead(pinLR) == true)
  {
    bRight = true;
  }
  else if (digitalRead(pinLL) == true)
  {
    bLeft = true;
  }
  lcd.clear();
  //Angle correction, turn until it detect the leg
  if (bRight == true)
  {
    writeLCD("Buscando RF", 0, 0);
    distRF = getDistance(pinSRF);
    while (distRF > 30)
    {
      turnNDegrees(iTurn, velLF + 8, velLB + 8, velRF, velRB);
      distRF = getDistance(pinSRF);
      if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
      {
        break;
      }
    }
  }
  else if (bLeft = true) {
    writeLCD("Buscando LF", 0, 0);
    distLF = getDistance(pinSLF);
    while (distLF > 30)
    {
      turnNDegrees(iTurn * -1, velLF, velLB, velRF + 8, velRB + 8);
      distLF = getDistance(pinSLF);
      if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
      {
        break;
      }
    }
  }

  //Correct the angle
  if (bRight == true)
  {
    writeLCD("Buscando LF", 0, 1);
    distLF = getDistance(pinSLF);
    while (distLF > 30)
    {
      turnNDegrees(iTurn, velLF, velLB, 0, 0);
      distLF = getDistance(pinSLF);
      if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
      {
        break;
      }
    }
  }
  else if (bLeft = true) {
    writeLCD("Buscando RF", 0, 1);
    distRF = getDistance(pinSRF);
    while (distRF > 30)
    {
      turnNDegrees(iTurn * -1, 0, 0, velRF, velRB);
      distRF = getDistance(pinSRF);
      if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
      {
        break;
      }
    }
  }

  if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
  {
    getInCow2();;
  }

  distRF = getDistance(pinSRF);
  distLF = getDistance(pinSLF);
  if (distRF > 30 && distLF < 30)
  {
    lcd.clear();
    writeLCD("Buscando RF", 0, 0);
    while (distRF > 30)
    {
      turnNDegrees(iTurn * -1, 0, 0, velRF, velRB);
      distRF = getDistance(pinSRF);
    }
  }
  else if (distRF < 30 && distLF > 30)
  {
    lcd.clear();
    writeLCD("Buscando LF", 0, 0);
    while (distLF > 30)
    {
      turnNDegrees(iTurn, velLF, velLB, 0, 0);
      distLF = getDistance(pinSLF);
    }
  }
  else {
    lcd.clear();
    writeLCD("Listo", 0, 0);
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
      if (digitalRead(pinLL) == 1 || digitalRead(pinLR) == 1)
      {
        getInCow2();
      }
    }

    brake();
  }
  if ((getDistance(pinSLF) > 30 && getDistance(pinSRF) < 30) || (getDistance(pinSRF) > 30 && getDistance(pinSLF) < 30))
  {
    getInCow2();
  }
}

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
*/

/*
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

    // //Move forward until back sharp lose the leg
    // forward(LF, LB, RF, RB);
    // while(distRB < 30)
    // {
    //   distRB = getDistance(pinSRB);
    // }
    // brake();

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
*/