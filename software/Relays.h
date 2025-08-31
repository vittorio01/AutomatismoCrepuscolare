#ifndef __RELAYS_H__
#define __RELAYS_H__

#include "Arduino.h"

typedef enum {START,STOP,OFF} counterStatus;

class Relays {
    private: 
    unsigned int number;
    unsigned int* counters;
    counterStatus* status;

    public:
    Relays(unsigned int relays);
    ~Relays();

    void setCount(unsigned int relay, unsigned int value,counterStatus direction);
    unsigned int getCount(unsigned int relay);
    counterStatus getDirection(unsigned int relay);

    void updateStatus();

};



#endif 
