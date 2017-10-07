#ifndef ABSTRACT_LOGGABLE_H
#define ABSTRACT_LOGGABLE_H

/**
 * Class to implement if you want to be used as a log.
 *
 * Note: The virtual/herency works when storing in variables 
 * pointers or references.
*/

#include "LevelLogger.h"

// #include <string>
// using namespace std;

class AbstractLoggable{
public:

    // Returns 0 if everything correct. Else if there is an error.
    virtual unsigned char init() = 0;
    virtual unsigned char getStatus() = 0;
    // TODO: Crear clase time para que el logger decida como formatearlo.
    virtual void log(const String &source, const LevelLogger &levelLog, const String &time,
        const String &header, const String &message) = 0;
    virtual unsigned char finish() = 0;

    static const unsigned char OK = 0;

};


#endif