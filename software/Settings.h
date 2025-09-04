#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "Arduino.h"
#include <EEPROM.h>

#define SETTINGS_CHECK_CHAR 0xAB
#define DEFAULT_POWER1      0
#define DEFAULT_POWER2      0
#define DEFAULT_POWER3      0
#define DEFAULT_POWER4      0
#define DEFAULT_TIMEROFF    10
#define DEFAULT_TIMERON     10
#define DEFAULT_BUZZER      true 

#define MAX_SETTINGS_BOUND  65536

using namespace std;

typedef struct SettingsValues{
    char errorCheck;
    unsigned int power1;
    unsigned int power2;
    unsigned int power3;
    unsigned int power4;

    unsigned int defaultTimerOff;
    unsigned int defaultTimerOn;

    bool buzzer;
} SettingsValues;

class Settings {
    private: 
    SettingsValues values;

    public:
    Settings();
    
    void loadDefault();
    bool loadSaved(unsigned int address);
    void store(unsigned int address);

    void setPower1(unsigned int value);
    void setPower2(unsigned int value);
    void setPower3(unsigned int value);
    void setPower4(unsigned int value);
    void setTimerOn(unsigned int value);
    void setTimerOff(unsigned int value);
    void setBuzzer(bool value);

    unsigned int getPower1();
    unsigned int getPower2();
    unsigned int getPower3();
    unsigned int getPower4();
    unsigned int getTimerOn();
    unsigned int getTimerOff();
    bool getBuzzer();
};


#endif 
