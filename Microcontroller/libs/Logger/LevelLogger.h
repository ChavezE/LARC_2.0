#ifndef LEVEL_LOGGER_H
#define LEVEL_LOGGER_H

/**
 * Clase usada para indicar en Logger.h, cual es el nivel
 * a usar para los logs.
 *
 * Probablente esto deberia ser un enum. E incluso un enum
 * miembro de Logger.h. O al menos esta clase ser friend de
 * Logger. Algun dia.
 * TODO: Ver viabilidad para cambiar a usar char* en vez 
 * de string.
*/

// #include <string>
// using namespace std;

class LevelLogger{
public:
    LevelLogger(const String &title, signed char level);

    //Because of this guy we cannot have the attributes as
    //const. Operator = is necessary for static instance of premade levels.
    LevelLogger& operator=(const LevelLogger &otherLogger);

    String getTitle();
    signed char getLevel();

    // Default Levels in logs
    static const LevelLogger ERROR;
    static const LevelLogger COMMUNICATION;
    static const LevelLogger WARNING;
    static const LevelLogger INFO;
    static const LevelLogger VERBOSE;

private:
    //Title to identify the log like Error, Warning, etc.
    String title;
    //Level of importance Smaller=most important, 
    //Bigger= least important. This can be negative in order to 
    //declare even more critical than default ones in Logger.h
    signed char level;
};


LevelLogger::LevelLogger(const String &title, signed char level):title(title), level(level) {

}

LevelLogger& LevelLogger::operator=(const LevelLogger &otherLogger) {
    this->title = otherLogger.title;
    this->level = otherLogger.level;
}

String LevelLogger::getTitle() {
    return title;
}

signed char LevelLogger::getLevel() {
    return level;
}

const LevelLogger LevelLogger::ERROR = LevelLogger("Error", -1);
const LevelLogger LevelLogger::COMMUNICATION = LevelLogger("Communication", 0);
const LevelLogger LevelLogger::WARNING = LevelLogger("Warning", 1);
const LevelLogger LevelLogger::INFO = LevelLogger("Info", 2);
const LevelLogger LevelLogger::VERBOSE = LevelLogger("Verbose", 3);

#endif