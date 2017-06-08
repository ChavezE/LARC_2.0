////////////////////////////////////////
//  Nestor Movment Functions          //
////////////////////////////////////////

//forward with P correction
void forwardP(int iWant, int& leftFront, int& leftBack, int& rightFront, int& rightBack, bool bSlow)
{
  //Actual angle
  int iAm = getCompass();
  //Know limits
  int iAux = iWant - iAm;

  //Reset vel to default
  leftFront = bSlow == false ? velLF : velSlowLF;
  leftBack = bSlow == false ? velLB : velSlowLB;
  rightFront = bSlow == false ? velRF : velSlowRF;
  rightBack = bSlow == false ? velRB : velSlowRB;

  //Debug info
  // Serial.print("iAm: ");
  // Serial.print(iAm);
  // Serial.print("\t");
  // Serial.print("iWant: ");
  // Serial.print(iWant);
  // Serial.print("\t");
  // Serial.print("iAux (iWant - iAm): ");
  // Serial.print(iAux);

  //Change speed if needed
  if (iAux > -180 && iAux < 180)
  {
    // Serial.print("\t");
    // Serial.print("Normal");
    if (iWant > iAm)
    {
      //Turn right
      leftFront += abs(iAux) / constPCorrectN;
      leftBack += abs(iAux) / constPCorrectN;

      // Serial.print("\t");
      // Serial.print("Mayor: ");
      // Serial.print(abs(iAux) / constPCorrectN);

      //Not needed
      // rightFront -= abs(iAux) / constPCorrectN;
      // rightBack -= abs(iAux) / constPCorrectN;
    }
    else if (iWant < iAm)
    {
      //Turn left
      rightFront += abs(iAux) / constPCorrectN;
      rightBack += abs(iAux) / constPCorrectN;

      // Serial.print("\t");
      // Serial.print("Menor: ");
      // Serial.print(abs(iAux) / constPCorrectN);

      //Not needed
      // leftFront -= abs(iAux) / constPCorrectN;
      // leftBack -= abs(iAux) / constPCorrectN;
    }
  }
  else if (iAux > 180 || iAux < -180)
  {
    // Serial.print("\t");
    // Serial.print("Anormal");
    if (iAux > 0)
    {
      //Ther shouldn't be any operation inside abs
      int iA = iAux - 360;

      // Serial.print("\t");
      // Serial.print("iA: ");
      // Serial.print(iA);
      // Serial.print("\t");
      // Serial.print("Mayor: ");
      // Serial.print(abs(iA) / constPCorrectN);

      //Turn left
      rightFront += abs(iA) / constPCorrectN;
      rightBack += abs(iA) / constPCorrectN;
      //Not needed
      // leftFront -= abs(iA) / constPCorrectN;
      // leftBack -= abs(iA) / constPCorrectN;
    }
    else if (iAux < 0)
    {
      //Ther shouldn't be any operation inside abs
      int iA = iAux + 360;

      // Serial.print("\t");
      // Serial.print("iA: ");
      // Serial.print(iA);
      // Serial.print("\t");
      // Serial.print("Menor: ");
      // Serial.print(abs(iA) / constPCorrectN);

      //Turn right
      leftFront += abs(iA) / constPCorrectN;
      leftBack += abs(iA) / constPCorrectN;
      //Not needed
      // rightFront -= abs(iA) / constPCorrectN;
      // rightBack -= abs(iA) / constPCorrectN;
    }
  }
  // Serial.println();
  // writeLCD(String(leftFront), 0, 0);
  // writeLCD(String(leftBack), 0, 1);
  // writeLCD(String(rightFront), 8, 0);
  // writeLCD(String(rightBack), 8, 1);
  //delay(300);
  setVelocity(leftFront, leftBack, rightFront, rightBack);
}

//forward with P correction
void backwardP(int iWant, int& leftFront, int& leftBack, int& rightFront, int& rightBack, bool bSlow)
{
  //Actual angle
  int iAm = getCompass();
  //Know limits
  int iAux = iWant - iAm;

  //Reset vel to default
  leftFront = bSlow == false ? velLF : velSlowLF;
  leftBack = bSlow == false ? velLB : velSlowLB;
  rightFront = bSlow == false ? velRF : velSlowRF;
  rightBack = bSlow == false ? velRB : velSlowRB;

  //Change speed if needed
  if (iAux > -180 && iAux < 180)
  {
    if (iWant > iAm)
    {
      //Turn right
      rightFront += abs(iAux) / constPCorrectN;
      rightBack += abs(iAux) / constPCorrectN;
      //Not needed
      // leftFront -= abs(iAux) / constPCorrect;
      // leftBack -= abs(iAux) / constPCorrect;
    }
    else if (iWant < iAm)
    {
      //Turn left
      leftFront += abs(iAux) / constPCorrectN;
      leftBack += abs(iAux) / constPCorrectN;
      //Not needed
      // rightFront -= abs(iAux) / constPCorrect;
      // rightBack -= abs(iAux) / constPCorrect;
    }
  }
  else if (iAux > 180 || iAux < -180)
  {
    if (iAux > 0)
    {
      //Ther shouldn't be any operation inside abs
      int iA = iAux - 360;
      //Turn left
      leftFront += abs(iA) / constPCorrectN;
      leftBack += abs(iA) / constPCorrectN;
      //Not needed
      // rightFront -= abs(iA) / constPCorrect;
      // rightBack -= abs(iA) / constPCorrect;
    }
    else if (iAux < 0)
    {
      //Ther shouldn't be any operation inside abs
      int iA = iAux + 360;
      //Turn right
      rightFront += abs(iA) / constPCorrectN;
      rightBack += abs(iA) / constPCorrectN;
      //Not needed
      // leftFront -= abs(iA) / constPCorrect;
      // leftBack -= abs(iA) / constPCorrect;
    }
  }
  setVelocity(leftFront, leftBack, rightFront, rightBack);
}

//Go forward the cm given in the parameter, Nestor style
void forwardNCm(int cm)
{
  //writeLCD("ForwardNCm", 0, 0);
  //writeLCD(String(cm), 0, 1);
  encoderState = 1;
  //Counts of encoder to get to the objective
  int untilSteps = (encoder30Cm / 30) * cm;
  //Restart encoder counts
  steps = 0;
  //Angle to stay in
  int iStayAngle = getCompass();

  //Start at default velocity
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;

  //Start moving
  forward(LF, LB, RF, RB);

  //Move with p correction until the encoder read the cm
  while (steps < untilSteps)
  {
    forwardP(iStayAngle, LF, LB, RF, RB, false);
  }
  //Stop
  brake();
}

//Go backward the cm given in the parameter, Nestor style
void backwardNCm(int cm)
{
  encoderState = 1;
  //Counts of encoder to get to the objective
  int untilSteps = (encoder30Cm / 30) * cm;
  //Restart encoder counts
  steps = 0;
  //Angle to stay in
  int iStayAngle = getCompass();

  //Start at default velocity
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;

  //Start moving
  backward(LF, LB, RF, RB);

  while (steps < untilSteps)
  {
    backwardP(iStayAngle, LF, LB, RF, RB, false);
  }

  brake();
}

//Go forward until finding a wall at a certain distance
void forwardUntilWallN(int dist)
{
  encoderState = 1;

  //Angle to stay in
  int iStayAngle = getCompass();

  //Start at default velocity
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;

  //Distance by front sharp
  int actualDist = getDistance(pinSF);

  //Check if the function
  bool ready = actualDist == dist ? true : false;
  bool bSlow = actualDist >= 30 ? false : true;

  //While not at ceratin distance from wall
  while (!ready)
  {
    //To far from wall
    if (actualDist > dist)
    {
      brake();
      forward(LF, LB, RF, RB);
      forwardP(iStayAngle, LF, LB, RF, RB, bSlow);
    }

    //To close from wall
    if (actualDist < dist)
    {
      brake();
      backward(LF, LB, RF, RB);
      backwardP(iStayAngle, LF, LB, RF, RB, bSlow);
    }
    
    actualDist = getDistance(pinSF);
    bSlow = actualDist >= 30 ? false : true;
    
    //Already at the distance with an error of +- 2 cm.
    if (actualDist > dist - 2 && actualDist < dist + 2)
    {
      ready = true;
    }
  }

  brake();
}

//Go forward until finding a wall at a certain distance
void backwardUntilWallN(int dist)
{
  encoderState = 1;

  //Angle to stay in
  int iStayAngle = getCompass();

  //Start at default velocity
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;

  //Distance by front sharp
  int actualDist = getDistance(pinSB);

  //Check if the function
  bool ready = actualDist == dist ? true : false;

  bool bSlow = actualDist >= 30 ? false : true;

  //While not at ceratin distance from wall
  while (!ready)
  {
    //To far from wall
    if (actualDist > dist)
    {
      backward(LF, LB, RF, RB);
      backwardP(iStayAngle, LF, LB, RF, RB, bSlow);
    }

    //To close from wall
    if (actualDist < dist)
    {
      forward(LF, LB, RF, RB);
      forwardP(iStayAngle, LF, LB, RF, RB, bSlow);
    }

    actualDist = getDistance(pinSB);
    bSlow = actualDist >= 30 ? false : true;


    //Already at the distance with an error of +- 2 cm.
    if (actualDist > dist - 2 && actualDist < dist + 2)
    {
      ready = true;
    }
  }

  brake();
}

//Turn to an exact angle
void turnToObjectiveN(int iWant)
{
  //Actual Angle
  int iAm = getCompass();
  //Auxiliar to know limits
  int iAux = iWant - iAm;

  // +- Error
  int iError = 1;

  while(!(iAm > iWant - iError && iAm < iWant + iError))
  {
    //Move to the angle in the shorter path
    if (iAux > -180 && iAux < 180)
    {
      if (iWant > iAm)
      {
        //Turn right
        turnRight(velLF, velLB, velRF, velRB);
      }
      else if (iWant < iAm)
      {
        //Turn left
        turnLeft(velLF, velLB, velRF, velRB);
      }
    }
    else if (iAux > 180 || iAux < -180)
    {
      if (iAux > 0)
      {
        //Turn left
        turnLeft(velLF, velLB, velRF, velRB);
      }
      else if (iAux < 0)
      {
        //Turn right
        turnRight(velLF, velLB, velRF, velRB);
      }
    }
    
    //check angle
    iAm = getCompass();
    iAux = iWant - iAm;
  }
  brake();
}

//Turn n amount of degrees, positive turn right, negative turn left
void turnNDegrees(int n)
{
  //Get objective angle
  int obj = getCompass() + n;

  //angle correction
  if (obj > 359)
  {
    obj -= 360;
  }
  else if (obj < 0)
  {
    obj += 360;
  }

  //Turn to degree
  turnToObjectiveN(obj);
}


////////////////////////////////////////////////////
//                    Faltarian:                  //
//                                                //
//    (Forward/Backward)(TillNoLeft/TillNoRight)  //
////////////////////////////////////////////////////