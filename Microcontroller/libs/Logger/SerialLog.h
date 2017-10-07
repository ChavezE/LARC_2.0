#ifndef SERIAL_LOG_H
#define SERIAL_LOG_H

/**
 * Class to log to the serial
 *
*/

#include "Arduino.h"

#include "LevelLogger.h"
#include "AbstractLoggable.h"

// #include <string>
// using namespace std;

class SerialLog : public AbstractLoggable{
public:
    SerialLog();
    SerialLog(const HardwareSerial &serialTo);
    SerialLog(const HardwareSerial &serialTo, const long &baudRate);

    // Returns 0 if everything correct. Else if there is an error.
    unsigned char init();
    unsigned char getStatus();
    void log(const String &source, const LevelLogger &levelLog, const String &time,
        const String &header, const String &message);
    unsigned char finish();

    static const unsigned char NOT_INIT = 1;

private:
    const HardwareSerial &serialTo;
    const long baudRate;
    unsigned char status;

};

SerialLog::SerialLog() : serialTo(Serial), baudRate(9600) {
    status = SerialLog::NOT_INIT;
}

SerialLog::SerialLog(const HardwareSerial &serialTo) : serialTo(serialTo), baudRate(9600) {
    status = SerialLog::NOT_INIT;
}

SerialLog::SerialLog(const HardwareSerial &serialTo, const long &baudRate) : serialTo(serialTo), baudRate(baudRate) {
    status = SerialLog::NOT_INIT;
}


unsigned char SerialLog::init() {
    serialTo.begin(baudRate);
    status = SerialLog::OK;

    return SerialLog::OK;
}

unsigned char SerialLog::getStatus() {
    return status;
}

void SerialLog::log(const String &source, const LevelLogger &levelLog, const String &time,
    const String &header, const String &message) {
    if (status != SerialLog::OK) {

    } else {
        serialTo.println("[" + source + "-" + levelLog.getTitle() + "]" + time + " [" + header + "] " + message);
        //"[Mega1-Error]10:03:80 [Main] assertEquals Failed: 1,2. En loop x=10" ESTE es el bueno
    }
}

unsigned char SerialLog::finish() {
    serialTo.end();
    status = SerialLog::NOT_INIT;

    return SerialLog::OK;
}


#endif