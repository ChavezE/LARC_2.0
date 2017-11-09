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
    static int distFromWall = 6;
    // Distance that will be add for the next terrine
    const int distAddEachTime = 7;


    turnToObjectiveN(iWest);
    // Pass the gate
    forwardNCm(50, false); // TODO: Maybe is better to pass until pass the sharpLeftBack

    // Going until the terrine zone to leave it
    logger.log("Forw with Left");
    delay(1000);
    int distFront, distBack;
    do {
        forwardWithLeftWall(iWest, 17, false, distFront, distBack);
    } while (distFront < 30);
    brake();

    turnToObjectiveN(iWest); // TODO: It is good idea to check, after turn, the sharpLeftBack if it is very far to return and get closer
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
    backwardNCm(60, false);
    backwardUntilNoLeft(iWest);
    backwardNCm(5, false);
    

    distFromWall += distAddEachTime;


    logger.log("Finish");
    delay(2000);
}
