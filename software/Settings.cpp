#include "Settings.h"
#include "Arduino.h"

using namespace std;

Settings::Settings() {
    loadDefault();
}

bool Settings::loadSaved(unsigned int address) {
    EEPROM.get(address,values);
    if (values.errorCheck!=SETTINGS_CHECK_CHAR) {
        loadDefault();
        return false;;
    }
    return true;
}

void Settings::store(unsigned int address) {
    if (values.errorCheck!=SETTINGS_CHECK_CHAR) {
        EEPROM.put(address,&values);
    }
}

void Settings::loadDefault() {
    values.errorCheck=SETTINGS_CHECK_CHAR;
    values.power1=DEFAULT_POWER1;
    values.power2=DEFAULT_POWER2;
    values.power3=DEFAULT_POWER3;
    values.power4=DEFAULT_POWER4;
    values.defaultTimerOff=DEFAULT_TIMEROFF;
    values.defaultTimerOn=DEFAULT_TIMERON;
    values.buzzer=DEFAULT_BUZZER;
}

void Settings::setPower1(unsigned int value) {
    values.power1=value;
}
void Settings::setPower2(unsigned int value) {
    values.power2=value;
}
void Settings::setPower3(unsigned int value) {
    values.power3=value;
}
void Settings::setPower4(unsigned int value) {
    values.power4=value;
}
void Settings::setTimerOn(unsigned int value) {
    values.defaultTimerOn=value;
}
void Settings::setTimerOff(unsigned int value) {
    values.defaultTimerOff=value;
}
void Settings::setBuzzer(bool value) {
    values.buzzer=value;
}

unsigned int Settings::getPower1() {
    return values.power1;
}
unsigned int Settings::getPower2() {
    return values.power2;
}
unsigned int Settings::getPower3() {
    return values.power3;
}
unsigned int Settings::getPower4() {
    return values.power4;
}
unsigned int Settings::getTimerOn() {
    return defaultTimerOn;
}
unsigned int Settings::getTimerOff() {
    return defaultTimerOff;
}
bool Settings::getBuzzer() {
    return buzzer;
}
