#include "Arduino.h"
#include "Relays.h"

Relays::Relays(unsigned int relays) {
    if (relays==0) return;
    number=relays;
    counters= new unsigned int [number];
    for (unsigned int r=0;r<number;r++) {
        counters[r]=0;
    }
}
Relays::~Relays() {
    if (number>0) {
        delete [] counters;
        delete [] status;
    }
}

void setCount(unsigned int relay, unsigned int value,counterStatus direction);
unsigned int getCount(unsigned int relay);
counterStatus getDirection(unsigned int relay);

void updateStatus();