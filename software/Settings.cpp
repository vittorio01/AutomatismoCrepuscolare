#include "Settings.h"
#include "Arduino.h"

Settings::Settings(unsigned int relays) {
    priorities=new unsigned short[relays];
Settings::~Settings() {
    delete [] properties;
}