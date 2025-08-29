#include "Arduino.h"
#include "Sensor.h"

Sensor::Sensor(int pin) {
    sensorPin=pin;
}

unsigned int Sensor::getCurrentPower() {
    return B+(digitalRead(sensorPin)*C);
}