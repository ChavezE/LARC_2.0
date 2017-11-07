/**
 * Function to leave the terrine in the empty terrine zone.
 * It starts in the gate looking at the north in the 
 * grasp zone. It finishes also in the gate, grasp, north.
 *
 */
void leaveTerrine () {
    SerialLog serialLogger;
    //serialLogger.init();
    LCDLogger lcdLogger;
    lcdLogger.init();
  
    AbstractLoggable *loggerArray[2]{&lcdLogger, &serialLogger};
    Logger logger("Mega", "LeaveTerrine", LevelLogger::INFO, loggerArray, 1);

    logger.log("Leave Terrine");
    delay(2000);
    
  
    // Distance that is next from the front wall to leave 
    //  the terrine.
    static int distFromWall = 5;
    // Distance that will be add for the next terrine
    const int distAddEachTime = 5;


    turnToObjectiveN(iWest);
    forwardNCm(50, false); // Pass the gate

    // Going until the terrine zone to leave it
    logger.log("Forw with Left");
    delay(1000);
    int distFront, distBack;
    do {
        forwardWithLeftWall(iWest, 17, false, distFront, distBack);
    } while (distFront < 30);
    brake();
    logger.log("Forw until Cms");
    delay(1000);
    forwardUntilWallN(distFromWall);

    // Leave the terrine
    logger.log("Leaving Terrine");
    delay(1000);
    upClaw(); // We need to pass over the tube supports (up, out, down)
    platOut();
    downClaw();
    openClaw();

    //Return position
    logger.log("Returning");
    delay(1000);
    clawToStartPoint();
    backwardUntilNoLeft(iWest);
    backwardNCm(5, false);
    
    turnToObjectiveN(iNorth);

    distFromWall += distAddEachTime;


    logger.log("Finish");
    delay(2000);
}
