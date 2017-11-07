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
  

}