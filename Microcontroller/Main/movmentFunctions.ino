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
  // //writeLCD(String(leftFront), 0, 0);
  // //writeLCD(String(leftBack), 0, 1);
  // //writeLCD(String(rightFront), 8, 0);
  // //writeLCD(String(rightBack), 8, 1);
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
void forwardNCm(int cm, bool slow)
{
  //lcd.clear();
  //writeLCD("ForwardNCm", 0, 0);
  //writeLCD(String(cm), 0, 1);
  encoderState = 1;
  //Counts of encoder to get to the objective
  int untilSteps = (encoder30Cm / 30) * cm;
  //Restart encoder counts
  steps = 0;
  //Angle to stay in
  int iStayAngle = getCompass();

  int LF;
  int LB;
  int RF;
  int RB;
  //Start at default velocity
  if (slow == false)
  {
    LF = velLF;
    LB = velLB;
    RF = velRF;
    RB = velRB;
  }
  else
  {
    LF = velSlowLF;
    LB = velSlowLB;
    RF = velSlowRF;
    RB = velSlowRB;
  }

  //Start moving
  forward(LF, LB, RF, RB);

  //Move with p correction until the encoder read the cm
  while (steps < untilSteps)
  {
    forwardP(iStayAngle, LF, LB, RF, RB, slow);
  }
  //Stop
  brake();
}

//Go backward the cm given in the parameter, Nestor style
void backwardNCm(int cm, bool slow)
{
  //lcd.clear();
  //writeLCD("backwardNCm", 0, 0);
  //writeLCD(String(cm), 0, 1);

  encoderState = 1;
  //Counts of encoder to get to the objective
  int untilSteps = (encoder30Cm / 30) * cm;
  //Restart encoder counts
  steps = 0;
  //Angle to stay in
  int iStayAngle = getCompass();

  int LF;
  int LB;
  int RF;
  int RB;

  //Start at default velocity
  if (slow == false)
  {
    LF = velLF;
    LB = velLB;
    RF = velRF;
    RB = velRB;
  }
  else
  {
    LF = velSlowLF;
    LB = velSlowLB;
    RF = velSlowRF;
    RB = velSlowRB;
  }

  //Start moving
  backward(LF, LB, RF, RB);

  while (steps < untilSteps)
  {
    backwardP(iStayAngle, LF, LB, RF, RB, slow);
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
      forward(LF, LB, RF, RB);
      forwardP(iStayAngle, LF, LB, RF, RB, bSlow);
    }

    //To close from wall
    if (actualDist < dist)
    {
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
  //lcd.clear();
  //writeLCD("Turning To:", 0, 0);
  //writeLCD(String(iWant), 0, 1);

  //Actual Angle
  int iAm = getCompass();
  //Auxiliar to know limits
  int iAux = iWant - iAm;

  // +- Error
  int iError = 2;

  while (!(iAm > iWant - iError && iAm < iWant + iError))
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
    else //if (iAux > 180 || iAux < -180)
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
  //lcd.clear();
  //writeLCD(String(getCompass()), 0, 0);
}

//Turn to an exact angle
void turnToObjectiveN(int iWant, int vLF, int vLB, int vRF, int vRB)
{
  //Actual Angle
  int iAm = getCompass();
  //Auxiliar to know limits
  int iAux = iWant - iAm;

  // +- Error
  int iError = 2;

  while (!(iAm > iWant - iError && iAm < iWant + iError))
  {
    //Move to the angle in the shorter path
    if (iAux > -180 && iAux < 180)
    {
      if (iWant > iAm)
      {
        //Turn right
        turnRight(vLF, vLB, vRF, vRB);
      }
      else if (iWant < iAm)
      {
        //Turn left
        turnLeft(vLF, vLB, vRF, vRB);
      }
    }
    else if (iAux > 180 || iAux < -180)
    {
      if (iAux > 0)
      {
        //Turn left
        turnLeft(vLF, vLB, vRF, vRB);
      }
      else if (iAux < 0)
      {
        //Turn right
        turnRight(vLF, vLB, vRF, vRB);
      }
    }

    //check angle
    iAm = getCompass();
    iAux = iWant - iAm;
  }
  brake();
  //lcd.clear();
  //writeLCD(String(getCompass()), 0, 0);
}

//Turn n amount of degrees, positive turn right, negative turn left
void turnNDegrees(int n)
{
  if (n != 0) {
    ////writeLCD(String(n), 0, 0);
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
}

//Turn n amount of degrees, positive turn right, negative turn left
void turnNDegrees(int n, int vLF, int vLB, int vRF, int vRB)
{
  ////writeLCD(String(n), 0, 0);
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
  turnToObjectiveN(obj, vLF, vLB, vRF, vRB);
}

void forwardUntilNoRight()
{
  encoderState = 1;

  //Angle to stay in
  int iStayAngle = getCompass();

  //Start at default velocity
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;

  //Distance by right back sharp
  int distBack = getDistance(pinSRB);

  //Distance by right front sharp
  int distFront = getDistance(pinSRF);

  //Check if the the robot should go slower
  bool bSlow = distFront > 30 ? true : false;

  if (distBack < 30)
  {
    //Start moving
    forward(LF, LB, RF, RB);

    //While not at ceratin distance from wall
    while (distBack < 30)
    {
      forwardP(iStayAngle, LF, LB, RF, RB, bSlow);

      distFront = getDistance(pinSRF);
      bSlow = distFront > 30 ? true : false;

      distBack = getDistance(pinSRB);
    }
  }

  brake();
}

void forwardUntilNoLeft()
{
  encoderState = 1;

  //Angle to stay in
  int iStayAngle = getCompass();

  //Start at default velocity
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;

  //Distance by left back sharp
  int distBack = getDistance(pinSLB);

  //Distance by left front sharp
  int distFront = getDistance(pinSLF);

  //Check if the the robot should go slower
  bool bSlow = distFront > 30 ? true : false;

  if (distBack < 30)
  {
    //Start moving
    forward(LF, LB, RF, RB);

    //While not at ceratin distance from wall
    while (distBack < 30)
    {
      forwardP(iStayAngle, LF, LB, RF, RB, bSlow);

      distFront = getDistance(pinSLF);
      bSlow = distFront > 30 ? true : false;

      distBack = getDistance(pinSLB);
      Serial.println(distBack);
    }
  }
  brake();
}

void backwardUntilNoRight()
{
  encoderState = 1;

  //Angle to stay in
  int iStayAngle = getCompass();

  //Start at default velocity
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;

  //Distance by right back sharp
  int distBack = getDistance(pinSRB);

  //Distance by right front sharp
  int distFront = getDistance(pinSRF);

  //Check if the the robot should go slower
  bool bSlow = distBack > 30 ? true : false;

  if (distFront < 30)
  {
    //Start moving
    backward(LF, LB, RF, RB);

    //While not at ceratin distance from wall
    while (distFront < 30)
    {
      backwardP(iStayAngle, LF, LB, RF, RB, bSlow);

      distBack = getDistance(pinSRB);
      bSlow = distBack > 30 ? true : false;

      distFront = getDistance(pinSRF);
    }
  }
  brake();
}

void backwardUntilNoLeft()
{
  encoderState = 1;

  //Angle to stay in
  int iStayAngle = getCompass();

  //Start at default velocity
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;

  //Distance by left back sharp
  int distBack = getDistance(pinSLB);

  //Distance by left front sharp
  int distFront = getDistance(pinSLF);

  //Check if the the robot should go slower
  bool bSlow = distBack > 30 ? true : false;

  if (distFront < 30)
  {
    //Start moving
    backward(LF, LB, RF, RB);

    //While not at ceratin distance from wall
    while (distFront < 30)
    {
      backwardP(iStayAngle, LF, LB, RF, RB, bSlow);

      distBack = getDistance(pinSLB);
      bSlow = distBack > 30 ? true : false;

      distFront = getDistance(pinSLF);
    }
  }

  brake();
}

void goToStart()
{
  //Go back to try get to the venter of the actual area
  backwardNCm(60, false);
  //Turn to where the gate is
  turnToObjectiveN(iSouth);

  //Velocity of movment
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;
  //Distances from sharp
  int distF = getDistance(pinSF);
  int distRF = getDistance(pinSRF);
  int distLF = getDistance(pinSLF);

  //the leg that touch was the right one
  bool bRight = digitalRead(pinLR);
  //The leg that touch was the left one
  bool bLeft = digitalRead(pinLL);

  //Start moving
  forward(LF, LB, RF, RB);
  
  //Keep moving forward until we face a wall, collapse with one or pass one
  while(!bRight && !bLeft && distF > 25 && distRF > 25 && distLF > 25)
  {
    //P Correction
    forwardP(iSouth, LF, LB, RF, RB, false);
    //Limit swtiches
    bRight = digitalRead(pinLR);
    bLeft = digitalRead(pinLL);
    //Sharp
    distF = getDistance(pinSF);
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
  }
  //Stop moving
  brake();
  //Lecture that made the robot stop
  String sReason = "";
  if(distLF <= 25)
  {
    sReason = "distLF";
  }
  else if(distRF <= 25)
  {
    sReason = "disrRF";
  }
  else if (distF <= 25)
  {
    sReason = "distF";
  }
  else if(bRight)
  {
    sReason = "bRight";
  }
  else
  {
    sReason = "bLeft";
  }
  //Write reason in LCD
  lcd.clear();
  writeLCD("Por: " + sReason, 0, 0);
  
  //Get away from the wall
  backwardNCm(10, false);
  //Fix the angle
  turnToObjectiveN(iSouth);

  //If we collapse with the wall or get in front of one
  if(!(distRF <= 25 || distLF <= 25) || bRight || bLeft || distF <= 25)
  {
    //Keep looking for the gate
    bool bContinue = true;
    while(bContinue)
    {
      //Turn West to find the gate
      turnToObjectiveN(iWest);
      //Set velocities
      LF = velLF;
      LB = velLB;
      RF = velRF;
      RB = velRB;
      //Sharp
      distLF = getDistance(pinSLF);
      //Limit read
      bLeft = digitalRead(pinLL);
      bRight = digitalRead(pinLR);
      //Start moving
      forward(LF, LB, RF, RB);
      //Keep moving until we collapse with a wall or find a gape in the left side
      while(distLF <= 30 && !bLeft && !bRight)
      {
        //P Correction
        forwardP(iWest, LF, LB, RF, RB, false);
        //Limit Switches
        bRight = digitalRead(pinLR);
        bLeft = digitalRead(pinLL);   
        //Sharp
        distLF = getDistance(pinSLF);
      }
      brake();
      //If we find the gate
      if(distLF > 30)
      {
        int distLB = getDistance(pinSLB);
        //Keep moving until the robot get at the center of the gate
        while(distLB <= 30)
        {
          //P Correction
          forwardP(iWest, LF, LB, RF, RB, false);
          //Sharp
          distLB = getDistance(pinSLB);
        }
        //Stop moving
        brake();
        //We find the gate
        bContinue = false;
      }
      //If we collapse
      else
      {
        //Get away from the wall we collapse with
        backwardNCm(7, false);
        //Turn to the other side
        turnToObjectiveN(iEast);
        //Set start velocities
        LF = velLF;
        LB = velLB;
        RF = velRF;
        RB = velRB;
        //Sharp distance
        distRF = getDistance(pinSRF);
        //Limit read
        bLeft = digitalRead(pinLL);
        bRight = digitalRead(pinLR);
        //Start moving
        forward(LF, LB, RF, RB);
        //Keep moving until we collaps with a wall or find a gape at the right
        while(distRF < 30 && !bLeft && !bRight)
        {
          //P Correction
          forwardP(iEast, LF, LB, RF, RB, false);
          //Limit Switches
          bRight = digitalRead(pinLR);
          bLeft = digitalRead(pinLL);   
          //Sharp
          distRF = getDistance(pinSRF);
        }
        //Stop moving
        brake();
        //If we find a gape
        if(distRF > 30)
        {
          //Start moving
          forward(LF, LB, RF, RB);
          //Sharp distance
          int distRB = getDistance(pinSRB);
          //Keep moving until the robot get at the center of the gate
          while(distRB <= 30)
          {
            //P Correction
            forwardP(iEast, LF, LB, RF, RB, false);
            //Sharp
            distRB = getDistance(pinSRB);
          }
          //Stop moving
          brake();
          //We fid the gate
          bContinue = false;
        }
      }
      brake();  
    }
    brake(); 
  }
  brake();
  delay(500);
  turnToObjectiveN(iSouth);
}
