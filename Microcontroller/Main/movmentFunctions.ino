//forward with P correction
void forwardP(int iWant, int& leftFront, int& leftBack, int& rightFront, int& rightBack, bool bSlow)
{
  //Actual angle
  int iAm = getCompass();
  //Know limits
  int iAux = iWant - iAm;

  //Reset vel to default
  if (bSlow) {
    leftFront = velSlowLF;
    leftBack = velSlowLB;
    rightFront = velSlowRF;
    rightBack = velSlowRB;
  } else {
    leftFront = velLF;
    leftBack = velLB;
    rightFront = velRF;
    rightBack = velRB;
  }

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
      const int toAdd = abs(iAux) * constPCorrect; // TODO: Change the operation (for the sign) instead of using the abs()

      //Turn right
      leftFront += toAdd;
      leftBack += toAdd;

      rightFront -= toAdd;
      rightBack -= toAdd;

      // Serial.print("\t");
      // Serial.print("Mayor: ");
      // Serial.print(toAdd);
    }
    else if (iWant < iAm)
    {
      const int toAdd = abs(iAux) * constPCorrect;

      //Turn left
      rightFront += toAdd;
      rightBack += toAdd;

      leftFront -= toAdd;
      leftBack -= toAdd;

      // Serial.print("\t");
      // Serial.print("Menor: ");
      // Serial.print(toAdd);
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
      const int toAdd = abs(iA) * constPCorrect;

      // Serial.print("\t");
      // Serial.print("iA: ");
      // Serial.print(iA);
      // Serial.print("\t");
      // Serial.print("Mayor: ");
      // Serial.print(toAdd);

      //Turn left
      rightFront += toAdd;
      rightBack += toAdd;

      leftFront -= toAdd;
      leftBack -= toAdd;
    }
    else if (iAux < 0)
    {
      //Ther shouldn't be any operation inside abs
      int iA = iAux + 360;
      const int toAdd = abs(iA) * constPCorrect;

      // Serial.print("\t");
      // Serial.print("iA: ");
      // Serial.print(iA);
      // Serial.print("\t");
      // Serial.print("Menor: ");
      // Serial.print(toAdd);

      //Turn right
      leftFront += toAdd;
      leftBack += toAdd;

      rightFront -= toAdd;
      rightBack -= toAdd;
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
  if (bSlow) {
    leftFront = velSlowLF;
    leftBack = velSlowLB;
    rightFront = velSlowRF;
    rightBack = velSlowRB;
  } else {
    leftFront = velLF;
    leftBack = velLB;
    rightFront = velRF;
    rightBack = velRB;
  }

  //Change speed if needed
  if (iAux > -180 && iAux < 180)
  {
    if (iWant > iAm)
    {
      const int toAdd = abs(iAux) * constPCorrect;

      //Turn right
      rightFront += toAdd;
      rightBack += toAdd;

      leftFront -= toAdd;
      leftBack -= toAdd;
    }
    else if (iWant < iAm)
    {
      const int toAdd = abs(iAux) * constPCorrect;

      //Turn left
      leftFront += toAdd;
      leftBack += toAdd;

      rightFront -= toAdd;
      rightBack -= toAdd;
    }
  }
  else if (iAux > 180 || iAux < -180)
  {
    if (iAux > 0)
    {
      //Ther shouldn't be any operation inside abs
      int iA = iAux - 360;
      const int toAdd = abs(iA) * constPCorrect;

      //Turn left
      leftFront += toAdd;
      leftBack += toAdd;

      rightFront -= toAdd;
      rightBack -= toAdd;
    }
    else if (iAux < 0)
    {
      //Ther shouldn't be any operation inside abs
      int iA = iAux + 360;
      const int toAdd = abs(iA) * constPCorrect;

      //Turn right
      rightFront += toAdd;
      rightBack += toAdd;

      leftFront -= toAdd;
      leftBack -= toAdd;
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
// NOT TRIED IT with the new robot
// TODO: Add if it wants 0cms is until limits switches
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
  bool bSlow = abs(actualDist - dist) >= 15 ? false : true;

  int countCorrect = 0;
  //While not at ceratin distance from wall
  while (!ready)
  {
    //To far from wall
    if (actualDist > dist + 2)
    {
      forward(LF, LB, RF, RB); // TODO: Brake before changing the direction to avoid fast changes that turn badly the robot
      forwardP(iStayAngle, LF, LB, RF, RB, bSlow);
      countCorrect = 0;
    }
    else if (actualDist < dist - 2) //To close from wall
    {
      backward(LF, LB, RF, RB);
      backwardP(iStayAngle, LF, LB, RF, RB, bSlow);
      countCorrect = 0;
    }
    else //Already at the distance with an error of +- 2 cm.
    {
      brake();
      if (++countCorrect == 5) ready = true;
    }

    actualDist = getDistance(pinSF);
    bSlow = abs(actualDist - dist) >= 15 ? false : true;

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

  bool bSlow = abs(actualDist - dist) >= 15 ? false : true;

  int countCorrect = 0;
  //While not at ceratin distance from wall
  while (!ready)
  {
    //To far from wall
    if (actualDist > dist + 2)
    {
      backward(LF, LB, RF, RB);
      backwardP(iStayAngle, LF, LB, RF, RB, bSlow);
      countCorrect = 0;
    }
    else if (actualDist < dist - 2) //To close from wall
    {
      forward(LF, LB, RF, RB);
      forwardP(iStayAngle, LF, LB, RF, RB, bSlow);
      countCorrect = 0;
    }
    else //Already at the distance with an error of +- 2 cm.
    {
      brake();
      if (++countCorrect == 5) ready = true;
    }

    actualDist = getDistance(pinSB);
    bSlow = abs(actualDist - dist) >= 15 ? false : true;
  }

  brake();
}

//Turn to an exact angle
void turnToObjectiveN(int iWant)
{
  turnToObjectiveN(iWant, velTurnLF, velTurnLB, velTurnRF, velTurnRB);
}

//Turn to an exact angle
void turnToObjectiveN(int iWant, int vLF, int vLB, int vRF, int vRB)
{
  vLF = velTurnLF; // TODO: Remove the arguments to use always this
  vLB = velTurnLB;
  vRF = velTurnRF;
  vRB = velTurnRB;

  int x = 0;
  do {
    int diff = getAngleDifferenceD(iWant, getCompass());
    if (diff > 1) {
      turnLeft(vLF + diff * constPTurn, vLB + diff * constPTurn, vRF + diff * constPTurn, vRB + diff * constPTurn);
      x = 0;
    } else if (diff < -1) {
      turnRight(vLF - diff * constPTurn, vLB - diff * constPTurn, vRF - diff * constPTurn, vRB - diff * constPTurn);
      x = 0;
    } else {
      brake();
      if (++x == 5) break;
    }
    delay(30);
  } while(true);

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

  vLF = velTurnLF; // TODO: Remove the arguments to use always this
  vLB = velTurnLB;
  vRF = velTurnRF;
  vRB = velTurnRB;

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

/**
 * Bacward with P correction for being with certain distance with
 * the left distance.
 * It checks the compass, the sharp front and back difference and
 * the distance average betweeen the sharp front and back.
 *
 * @param {const int&} degreesObjetivo
 * @param {const int&} objetivoDistPared
 * @param {const bool&} slow
 * @param {int&} distFront The individual dist to the wall by the sharp front
 * @param {int&} distBack The dist sharp back
 *
 * @return {int} separacion The dist with the wall average with front and back
 */
int backwardWithLeftWall(const int& degreesObjetivo, const int& objetivoDistPared, const bool& slow, 
  int& distFront, int& distBack) {
  //Base for motors velocity
  int baseLF;
  int baseLB;
  int baseRF;
  int baseRB;
  if (slow) {
    baseLF = velSlowLF;
    baseLB = velSlowLB;
    baseRF = velSlowRF;
    baseRB = velSlowRB;
  } else {
    baseLF = velLF;
    baseLB = velLB;
    baseRF = velRF;
    baseRB = velRB;
  }

  distFront = getDistance(pinSLF);
  distBack = getDistance(pinSLB);

  const int separacion = (distFront + distBack) / 2;
  const int diffSeparacion = separacion - objetivoDistPared;
  const int diffSharps = distFront - distBack; // (-) voltear a la der
  const int diffCompass = getAngleDifferenceD(degreesObjetivo, getCompass()); // (-) conviene voltear a la derecha

  int velLF = formulaBackwardWithLeftWall(baseLF, -diffSeparacion, diffSharps, diffCompass);
  int velLB = formulaBackwardWithLeftWall(baseLB, -diffSeparacion, diffSharps, diffCompass);
  int velRF = formulaBackwardWithLeftWall(baseRF, diffSeparacion, -diffSharps, -diffCompass);
  int velRB = formulaBackwardWithLeftWall(baseRB, diffSeparacion, -diffSharps, -diffCompass);

  backward(velLF, velLB, velRF, velRB); // TODO: Use better setVelocity

  return separacion;
}

/**
 * Formula with the P taking the base, distance from the wall, difference from sharps
 * and diff from compass. Used in the backwardWithLeftWall.
 *
 */
int formulaBackwardWithLeftWall(int base, int separacion, int diffBetweenSharps, int diffCompass) {
  if (diffBetweenSharps > 45) diffBetweenSharps = 45;
  else if (diffBetweenSharps < -45) diffBetweenSharps = -45;
//  return base + separacion * 8 + diffBetweenSharps * 2 + diffCompass * 6;
  return base + separacion * 15 + diffBetweenSharps * 4 + diffCompass * 7; // * 2
//  return base + separacion * 12 + diffBetweenSharps * 2 + diffCompass * 7;
}

/**
 * Get the difference between the wanted angles and the actual angles.
 * Gives always the fastest direction and the smallest angles to turn to.
 * if result < 0 : best is to turn to the RIGHT
 * else if result > 0 : best is to turn to the LEFT
 * Always will be: 180 >= abs(result) >= 0
 *
 */
int getAngleDifferenceD(const int &objetivo, const int &actual) {
  int diffCompass = actual - objetivo;

  if (diffCompass > 180) {
    diffCompass = -( 180-(diffCompass-180) );
  } else if (diffCompass < -180) {
    diffCompass = ( 180-((-diffCompass)-180) );
  }

  return diffCompass; // (-) conviene voltear a la derecha
}

void parkingRight(bool bSlow)
{
  turnNDegrees(-20);
  backwardNCm(35, bSlow);
  turnNDegrees(20);
}

void parkingLeft(bool bSlow)
{
  turnNDegrees(20);
  backwardNCm(35, bSlow);
  turnNDegrees(-20);
}

void parkingRight(bool bSlow, int iCm)
{
  turnNDegrees(-20);
  backwardNCm(iCm, bSlow);
  turnNDegrees(20);
}

void parkingLeft(bool bSlow, int iCm)
{
  turnNDegrees(20);
  backwardNCm(iCm, bSlow);
  turnNDegrees(-20);
}
