/**
 * Main entrance for leave the terrine.
 *
 * Function to leave the terrine in the empty terrine zone.
 * It starts in the gate looking at the north in the 
 * grasp zone. It finishes in the grasp point to iWest ready for call
 * the basic (normal) version of goGrabTerrine.
 * This check if the terrines are in right or left (terrinesInitInRight).
 *
 */
void leaveTerrine() {
    SerialLog serialLogger;
    //serialLogger.init();
    LCDLogger lcdLogger;
    lcdLogger.init();
  
    AbstractLoggable *loggerArray[2]{&lcdLogger, &serialLogger};
    Logger logger("Mega", "LeaveTerrine", LevelLogger::INFO, loggerArray, 1);

    logger.log("Leave Terrine");
    delay(2000);

    if (terrinesInitInRight) {
        logger.log("LEFT");
        delay(1000);

        leaveTerrineLeft();
    } else {
        logger.log("RIGHT");
        delay(1000);

        leaveTerrineRight();
    }

}


/**
 * Leave terrine that goes to the left to leave it.
 * 
 * It starts in the gate in the grasp pointing to the north.
 * At the end it finishes at the gate in grasp pointing to the
 * west.
 */
void leaveTerrineLeft () {
    SerialLog serialLogger;
    //serialLogger.init();
    LCDLogger lcdLogger;
    lcdLogger.init();
  
    AbstractLoggable *loggerArray[2]{&lcdLogger, &serialLogger};
    Logger logger("Mega", "LeaveTerrLeft", LevelLogger::INFO, loggerArray, 1);

    logger.log("Leave Terr Left");
    delay(2000);
    
  
    // Distance that is next from the front wall to leave 
    //  the terrine.
    static int distFromWall = 6; // TODO: Maybe this two variables can be global to the both version of the functions
    // Distance that will be add for the next terrine
    const int distAddEachTime = 8;


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

/**
 * Leave terrine that goes to the right to leave it.
 * 
 * It starts in the gate in the grasp pointing to the north.
 * At the end it finishes at the gate in grasp pointing to the
 * west.
 */
 void leaveTerrineRight () {
 }