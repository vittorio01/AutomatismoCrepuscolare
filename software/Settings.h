#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "Arduino.h"
#include "EEPROM.h"

#define validChar   0xAB

class Settings {
    
    bool buzzer;
    unsigned short* priorities;
    
    Settings(unsigned short relays);
    ~Settings();

    void loadSettings();
    void saveSettings();
    void setPriority(unsigned short index,int value);
    
    void setBuzzer(bool value);

};


#endif 