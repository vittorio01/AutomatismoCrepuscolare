#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Arduino.h"

#define C   10
#define B   0

class Sensor {
    Private:
    unsigned int sensorPin;

    Public:
    Sensor(int pin);
    void getCurrentPower();
}

#endif 