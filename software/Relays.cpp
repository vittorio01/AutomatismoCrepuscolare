#include "Arduino.h"
#include "Relays.h"

Relays::Relays(unsigned int relays) {
    if (relays==0) return;
    number=relays;
    counters= new unsigned int [number];
    status= new counterStatus [number];
    for (unsigned int r=0;r<number;r++) {
        counters[r]=0;
        status[r]=OFF;
    }
}
Relays::~Relays() {
    if (number>0) {
        delete [] counters;
        delete [] status;
    }
}

void Relays::setCount(unsigned int relay, unsigned int value,counterStatus direction) {
    counters[relay]=value;
    status[relay]=direction;
}

unsigned int Relays::getCount(unsigned int relay) {
    return counters[relay];
}
counterStatus Relays::getDirection(unsigned int relay) {
    return status[relay];
}

void Relays::updateStatus() {
    for (unsigned int r=0;r<number;r++) {
        if (counters[r]>0) counters[r]--;
    }
}
