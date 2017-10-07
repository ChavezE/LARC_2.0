/**
 * Example program at the end.
*/

#ifndef LOGGER_H
#define LOGGER_H

#include "LevelLogger.h"
#include "AbstractLoggable.h"

#define MAX_LOGGERS 10

class Logger{
public:
    Logger(const String &source, const String &header, const LevelLogger &levelLogger, AbstractLoggable *logTo[], int cantLoggers);
    
    void log(const String &message);

private:
    const LevelLogger &levelLogger;
    AbstractLoggable *logTo[MAX_LOGGERS]; // TODO: Have the size of the array as a template?
    int cantLoggers;
    const String header;
    const String source;
};

Logger::Logger(const String &source, const String &header, const LevelLogger &levelLogger, 
    AbstractLoggable *logTo[], int cantLoggers) : source(source),
    header(header), levelLogger(levelLogger), cantLoggers(cantLoggers) {
    // if (cantLoggers > MAX_LOGGERS) valio;

    for (int x = 0; x < cantLoggers; x++) {
        this->logTo[x] = logTo[x];
    }

}


void Logger::log(const String &message) {
    String actual = String(micros());
    for (int x = 0; x < cantLoggers; x++) {
        logTo[x]->log(source, levelLogger, actual, header, message);
    }

}

#endif

/**

Example program

#include <Logger.h>
#include <SerialLog.h>
#include <LevelLogger.h>
#include <LCDLogger.h>

void setup() {
//  Serial.begin(9600);
//  Serial.println("dsadfdsa");
//  delay(0);
//Serial.println("sip");
//while(1);

  SerialLog serial;
  serial.init();
  LCDLogger lcd;
  lcd.init();

  AbstractLoggable *lista[2]{&serial, &lcd};
  Logger logger("Mega1", "Prueba", LevelLogger::INFO, lista, 2);
  
  logger.log("Holaa");
  delay(2000);
  logger.log("Holaaa");
  delay(2000);
  logger.log("Byee");
  delay(2000);
  
  Logger loggerError("Mega1", "Prueba", LevelLogger::ERROR, lista, 2);
  loggerError.log("Holaa");
  delay(2000);
  lcd.setDelayInLog(5000);
  loggerError.log("Holaa");
  //delay(2000);
  loggerError.log("Byee");
  delay(2000);

  serial.finish();
  lcd.finish();

}

void loop() {
  // put your main code here, to run repeatedly:

}

*/
