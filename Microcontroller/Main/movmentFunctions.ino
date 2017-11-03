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

/**
 * Check if we are crossing the gate.
 * First, it will only check if detecting something with sharp
 * left-front, if true it will stop and return to check if again 
 * sharp detects it. If true, we continue forwarding to look for the
 * the left-back sharp to find the gate.
 *
 * Note: We enter here already forwarding
 *
 * @param {Logger} logger Logger to log to
 *
 * @return {bool} true if all this is acomplished
 */
bool checkCrossingGate(const Logger &logger) {
  if (getDistance(pinSLF) <= 30) {
    // Return to check if we detect again the wall
    brake();
    logger.log("Wall LF");
    delay(1000);
    backwardNCm(8, false);

    int mientr1, mientr2, mientr3, mientr4;
    encoderState = 1;
    int untilSteps = (encoder30Cm / 30) * 9;
    steps = 0;
    forward(0,0,0,0);
    do {
      forwardP(iSouth, mientr1, mientr2, mientr3, mientr4, true);
    } while (getDistance(pinSLF) > 30 && steps < untilSteps);
    brake();
    delay(400);

    // We didnt find anything again
    if (steps >= untilSteps) {
      logger.log("Not found wall");
      delay(1000);
      backwardNCm(9, false);
      
      forward(0,0,0,0);
      return false;
    }
    
    // We found something, then lets look with sharp left-back
    // logger.log("Wall LF again");
    // delay(1000);
    // forward(0,0,0,0);
    // while (getDistance(pinSLB) > 30) {
    //   forwardP(iSouth, mientr1, mientr2, mientr3, mientr4, true);
    // }
    // brake();

    forwardNCm(45, false);

    return true;
  }

  return false;

}

void goToStart()
{
  LCDLogger lcdLogger;
  lcdLogger.init();
  SerialLog serialLogger;
  //serialLogger.init();

  AbstractLoggable *loggerArray[2]{&lcdLogger, &serialLogger};
  Logger logger("Mega", "ReturnCow", LevelLogger::INFO, loggerArray, 1);
  

  //Get out of the cow
  backwardNCm(30, false);
  //Turn to where the gate is
  turnToObjectiveN(iSouth);
  forwardNCm(35, false);

  //Velocity of movment
  int LF = velLF;
  int LB = velLB;
  int RF = velRF;
  int RB = velRB;
  //Distances from sharp
  int distF;
  int distRF;
  int distLF;

  bool letsContinue = true;
  
  logger.log("Forward to start");

  // We check if we are near from any side wall
  int distSideWalls = getDistance(pinSLF);
  if (distSideWalls < 32) {
    logger.log("Near left wall");
    delay(1000);
    
    turnToObjectiveN(iWest);
    forwardNCm(60 - distSideWalls, false);
  } else {
    distSideWalls = getDistance(pinSRF);
    if (distSideWalls < 32) {
      logger.log("Near left wall");
      delay(1000);
    
      turnToObjectiveN(iEast);
      forwardNCm(60 - distSideWalls, false);
    }
  }
  turnToObjectiveN(iSouth);

  //Start moving
  forward(LF, LB, RF, RB);
  //Keep moving forward until we face a wall, or detect a wall at right or left
  bool crossed;
  do {
    forwardP(iSouth, LF, LB, RF, RB, false);

    crossed = checkCrossingGate(logger);
  } while(!crossed && getDistance(pinSF) > 16 && digitalRead(pinLR) == HIGH && digitalRead(pinLL) == HIGH);
  brake();
  logger.log("Out of forward");
  delay(1000);

  if (crossed) {
    logger.log("End");
    return;
  }

  // if we didnt touch any limit, lets look horizontaly for the gate. 
  // Or maybe there was something strange to both.
  if (digitalRead(pinLR) == HIGH && digitalRead(pinLL) == HIGH || 
    digitalRead(pinLR) == LOW && digitalRead(pinLL) == LOW) {
    logger.log("Sharp detected");
    delay(1000);

    turnToObjectiveN(iWest); // Turn to the side of not terrines
    int backDist = getDistance(pinSLB), frontDist = getDistance(pinSLF);
    if (backDist < 30 && frontDist < 30) { //We are in the wall in side right or left
      logger.log("Wall right/left");
      delay(1000);

      // Backward until back wall or the gate
      backward(LF, LB, RF, RB);
      do
      {
        backwardP(iWest, LF, LB, RF, RB, false);
    
      } while(getDistance(pinSLB) < 35 && getDistance(pinSB) > 15); // TODO: CHANGE to read front to less probability of reading the terrine part
      brake();

      // See if arrive to the gate
      if (getDistance(pinSLB) >= 35) {
        logger.log("Half in gate");
        delay(1000);

        // Move until the gate completely
        backward(LF, LB, RF, RB);
        do {
          backwardP(iWest, LF, LB, RF, RB, true);
        } while(getDistance(pinSLF) < 35);
        brake();

        backwardNCm(5, false);

      } else { // We are at the left side lets look for the wall and leave
        logger.log("Other direction");
        delay(1000);

        // Move until the gate completely
        forward(LF, LB, RF, RB);
        do {
          forwardP(iWest, LF, LB, RF, RB, false);
        } while(getDistance(pinSLB) < 35);
        brake();

        forwardNCm(5, false);

      }

    } else if (backDist > 30 &&  frontDist < 30) {
      logger.log("Wall betw right");
      delay(1000);
  
      // Move until the gate completely
      backward(LF, LB, RF, RB);
      do {
        backwardP(iWest, LF, LB, RF, RB, true);
      } while(getDistance(pinSLF) < 35);
      brake();

      backwardNCm(5, false);
    } else if (backDist < 30 &&  frontDist > 30) {
      logger.log("Wall betw left");
      delay(1000);
      
      // Move until the gate completely
      forward(LF, LB, RF, RB);
      do {
        forwardP(iWest, LF, LB, RF, RB, true);
      } while(getDistance(pinSLB) < 35);
      brake();

      forwardNCm(5, false);
    }

    logger.log("En la gate");
    delay(1000);
    
    turnToObjectiveN(iSouth);
  }


  logger.log("Crossing Gate");
  delay(1000);

  // Cross until we pass both (front and back) of sharps
  // using parking if necessary (limit with wall).
  forward(0,0,0,0);
  do { // TODO: Maybe this can be join into one with the part of the beginning
    if (digitalRead(pinLL) == LOW) {
      brake();
      logger.log("Limit left");
      delay(1000);
  
      turnToObjectiveN(iSouth);
      parkingRight(false);
      forward(0,0,0,0);
    } else if (digitalRead(pinLR) == LOW) {
      brake();
      logger.log("Limit right");
      delay(1000);
      
      turnToObjectiveN(iSouth);
      parkingLeft(false);
      forward(0,0,0,0);
    }

    forwardP(iSouth, LF, LB, RF, RB, true);

    crossed = checkCrossingGate(logger); // TODO: Maybe we dont need this two time validation of sharp LF

  } while (!crossed);

  logger.log("END");
}

// Forward para ir derecho con cierta distancia a la pared derecha
int forwardWithRightWall(const int &degreesObjetivo, const int &objetivoDistPared, const bool &slow) {
  //Base for motors velocity
  const int baseLF = !slow ? velLF : velSlowLF;
  const int baseLB = !slow ? velLB : velSlowLB;
  const int baseRF = !slow ? velRF : velSlowRF;
  const int baseRB = !slow ? velRB : velSlowRB;

  const int front = getDistance(pinSRF);
  const int back = getDistance(pinSRB);

  const int separacion = (front + back) / 2;
  const int diffSeparacion = separacion - objetivoDistPared;
  const int diffSharps = front - back; // (-) voltear a la der
  const int diffCompass = getAngleDifferenceD(degreesObjetivo, getCompass()); // (-) conviene voltear a la derecha

  int velLF = formulaForwardWithRightWall(baseLF, diffSeparacion, diffSharps, -diffCompass);//formula(baseLF, -separacion, -separacionOrientado, -diffCompass);
  int velLB = formulaForwardWithRightWall(baseLB, diffSeparacion, diffSharps, -diffCompass);//formula(baseLB, -separacion, -separacionOrientado, -diffCompass);
  int velRF = formulaForwardWithRightWall(baseRF, -diffSeparacion, -diffSharps, diffCompass);//formula(baseRF, separacion, separacionOrientado, diffCompass);
  int velRB = formulaForwardWithRightWall(baseRB, -diffSeparacion, -diffSharps, diffCompass);//formula(baseRB, separacion, separacionOrientado, diffCompass);
  if (velLF > 255) velLF = 255;
  else if (velLF < 0) velLF = 0;
  if (velLB > 255) velLB = 255;
  else if (velLB < 0) velLB = 0;
  if (velRF > 255) velRF = 255;
  else if (velRF < 0) velRF = 0;
  if (velRB > 255) velRB = 255;
  else if (velRB < 0) velRB = 0;

  forward(velLF, velLB, velRF, velRB);

  return separacion;
}

/**
 * Formula with the P taking the base, distance from the wall, difference from sharps
 * and diff from compass. Used in the forwardWithRightWall.
 *
 */
int formulaForwardWithRightWall(int base, int separacion, int diffBetweenSharps, int diffCompass) {
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
