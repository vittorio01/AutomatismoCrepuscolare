#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "Arduino.h"

class Sensor {
    private:
    unsigned int sensorPin;
    unsigned int b;
    unsigned int c;

    public:
    Sensor(int pin,unsigned int base, unsigned int increment);
    unsigned int getCurrentPower();
};

#endif 
