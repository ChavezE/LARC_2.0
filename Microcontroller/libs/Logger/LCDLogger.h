#ifndef LCD_LOGGER_H
#define LCD_LOGGER_H

/**
 * Class to log to the LCD of 2 rows and 16 colums
 *
*/

#include "Arduino.h"

#include "LevelLogger.h"
#include "AbstractLoggable.h"
#include <LiquidCrystal_I2C.h>

// #include <string>
// using namespace std;

class LCDLogger : public AbstractLoggable{
public:
    // We create and set to default params and later will init the LCD object
    LCDLogger();
    LCDLogger(const unsigned char lcd_Addr, const unsigned char lcd_cols, const unsigned char lcd_rows);
    LCDLogger(const unsigned char lcd_Addr, const unsigned char lcd_cols, const unsigned char lcd_rows, 
        unsigned long delayInLog);

    // Returns 0 if everything correct. Else if there is an error.
    unsigned char init();
    unsigned char getStatus();
    void log(const String &source, const LevelLogger &levelLog, const String &time,
        const String &header, const String &message);
    unsigned char finish();

    const LiquidCrystal_I2C& getLCD();
    unsigned long getDelayInLog();
    void setDelayInLog(unsigned long delay);

    static const unsigned char NOT_INIT = 1;

private:
    void writeLCD(String word, int iCol, int iRow);

    const LiquidCrystal_I2C LCD;
    unsigned char status;
    unsigned long delayInLog;

};

LCDLogger::LCDLogger() : LCD(LiquidCrystal_I2C(0x27, 16, 2)) {
    status = LCDLogger::NOT_INIT;
    delayInLog = 0UL;
}

LCDLogger::LCDLogger(const unsigned char lcd_Addr, const unsigned char lcd_cols, const unsigned char lcd_rows) :
    LCD(LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows)) {
    status = LCDLogger::NOT_INIT;
    delayInLog = 0UL;
}

LCDLogger::LCDLogger(const unsigned char lcd_Addr, const unsigned char lcd_cols, const unsigned char lcd_rows, 
    unsigned long delayInLog) : LCD(LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows)) {
    status = LCDLogger::NOT_INIT;
    this->delayInLog = delayInLog;
}

unsigned char LCDLogger::init() {
    LCD.init();
    LCD.backlight();
    LCD.clear();
    LCD.home();

    status = AbstractLoggable::OK;

    return AbstractLoggable::OK;
}

unsigned char LCDLogger::getStatus() {
    return status;
}

void LCDLogger::log(const String &source, const LevelLogger &levelLog, const String &time,
    const String &header, const String &message){
    if (status != SerialLog::OK) {

    } else {
        LCD.clear();
        String mientr;
        if (levelLog.getTitle().length() > 0) mientr = levelLog.getTitle()[0];
        else mientr = "";
        mientr += "-" + (header.length() > 12 ? header.substring(0, 12) : header);
        writeLCD(mientr, 0, 0);

        mientr = message.length() > 15? message.substring(0, 15) : message;
        writeLCD(message, 0, 1);
        //E-Main 
        //Entrando vaca
        if (delayInLog > 0UL) delay(delayInLog);
    }
}

unsigned char LCDLogger::finish() {
    // Nothing

    return SerialLog::OK;
}


const LiquidCrystal_I2C& LCDLogger::getLCD() {
    return LCD;
}

unsigned long LCDLogger::getDelayInLog() {
    return delayInLog;
}

void LCDLogger::setDelayInLog(unsigned long delay) {
    delayInLog = delay;
}


void LCDLogger::writeLCD(String word, int iCol, int iRow)
{
  bool firstLine = true;
  for(int iI = 0; iI < word.length(); iI++)
  {
    if(iI > 15 && firstLine == true)
    {
      iRow = 1;
      iCol = 0;
      firstLine = false;
    }
    LCD.setCursor(iCol, iRow);
    LCD.print(word[iI]);
    iCol++;
  }
}

#endif