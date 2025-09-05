#ifndef __RELAYS_H__
#define __RELAYS_H__

#include "Arduino.h"

typedef enum {START,STOP,OFF,ON} counterStatus;

class Relays {
    private: 
    unsigned int number;
    unsigned int* counters;
    counterStatus* status;
    bool* changedFlags;

    public:
    Relays(unsigned int relays);
    ~Relays();

    void setCount(unsigned int relay, unsigned int value,counterStatus direction);
    unsigned int getCount(unsigned int relay);
    counterStatus getDirection(unsigned int relay);

    bool getChanged(unsigned int relay);
    void clearFlag(unsigned int relay);

    void updateStatus();

};



#endif 
