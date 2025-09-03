#include "Display.h"
#include "Settings.h"
#include "Sensor.h"
#include "Relays.h"

#include <TimerOne.h>

#define LCD_SHIFT_EN              4
#define LCD_SHIFT_D7              5
#define LCD_SHIFT_SER             6
#define LCD_SHIFT_CLK             7

#define LCD_FORMAT_ROWS           2
#define LCD_FORMAT_COLS           20
#define LCD_MAX_COLS              38
#define SETTINGS_ADDRESS          0

#define PHOTO_PIN                 A0
#define PHOTO_BASEVALUE           0
#define PHOTO_INCREMENT           10

#define SELECT_BUTTON             2
#define MINUS_BUTTON              8
#define PLUS_BUTTON               9

#define RELAY1_PIN                0
#define RELAY2_PIN                1
#define RELAY3_PIN                11  
#define RELAY4_PIN                10

#define BUZZER_PIN                12
#define BUZZER_FREQUENCY          3500

#define DISPLAY_REFRESH_RATE      100000

Display* screen;
Settings* options;
Sensor* light;
Relays* counters;

void control();
void update();
void generateTone(int toneDelay);
void generate_alarm(int toneDelay);

typedef enum {GENERAL,SETTINGS} view;

view currentView;
bool blinkStatus;
unsigned int secondCounter;

void setup() {
  noInterrupts(); 

  pinMode(SELECT_BUTTON, INPUT);
  pinMode(MINUS_BUTTON, INPUT);
  pinMode(PLUS_BUTTON, INPUT);
  pinMode(PHOTO_PIN, INPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);

  digitalWrite(RELAY1_PIN, true);
  digitalWrite(RELAY2_PIN, true);
  digitalWrite(RELAY3_PIN, true);
  digitalWrite(RELAY4_PIN, true);

  currentView=GENERAL;
  blinkStatus=false;
  secondCounter=1000000/DISPLAY_REFRESH_RATE;

  screen=new Display(LCD_SHIFT_EN, LCD_SHIFT_D7, LCD_SHIFT_SER, LCD_SHIFT_CLK,LCD_FORMAT_ROWS,LCD_FORMAT_COLS,LCD_MAX_COLS);
  options=new Settings();
  light=new Sensor(PHOTO_PIN,PHOTO_BASEVALUE,PHOTO_INCREMENT);
  counters=new Relays(4);
  
  options->loadSaved(SETTINGS_ADDRESS);

    
  attachInterrupt(0, control,LOW);
  Timer1.initialize((DISPLAY_REFRESH_RATE/2));
  Timer1.attachInterrupt(update);
  interrupts(); 
}

void loop() {

}

void control() {
  currentView=SETTINGS;
  screen->clear();
  screen->write(0,0,"Potenza carico 1");
  screen->write(1,0,String((char) 0b00111110)+" ");
}

void update() {
  if (secondCounter>0) {
    secondCounter--;
  } else {
    secondCounter=1000000/DISPLAY_REFRESH_RATE;
    blinkStatus=!blinkStatus;
  }
  

  switch (currentView) {
    case GENERAL:
      screen->write(0,0,"Potenza:");
      screen->write(0,10,String(light->getCurrentPower())+"W");
      if (!digitalRead(RELAY1_PIN)) {
        screen->write(1,0,"ON");
      } else {
        screen->write(1,0,"OFF");
      }
      if (!digitalRead(RELAY2_PIN)) {
        screen->write(1,5,"ON");
      } else {
        screen->write(1,5,"OFF");
      }
      if (!digitalRead(RELAY1_PIN)) {
        screen->write(1,10,"ON");
      } else {
        screen->write(1,10,"OFF");
      }
      if (!digitalRead(RELAY1_PIN)) {
        screen->write(1,15,"ON");
      } else {
        screen->write(1,15,"OFF");
      }

      switch(counters->getDirection(0)) {
        case STOP:
        if(blinkStatus) {
          screen->write(1,2,String((char) 0b00011111));
        } else {
          screen->write(1,2,String(" "));
        }
        
        break;

        case START:
        if(blinkStatus) {
          screen->write(1,3,String((char) 0b00011110));
        } else {
          screen->write(1,3,String(" "));
        }
        break;
      }
      switch(counters->getDirection(1)) {
        case STOP:
        if(blinkStatus) {
          screen->write(1,7,String((char) 0b00011111));
        } else {
          screen->write(1,7,String(" "));
        }
        
        break;

        case START:
        if(blinkStatus) {
          screen->write(1,8,String((char) 0b00011110));
        } else {
          screen->write(1,8,String(" "));
        }
        break;
      }
      switch(counters->getDirection(2)) {
        case STOP:
        if(blinkStatus) {
          screen->write(1,12,String((char) 0b00011111));
        } else {
          screen->write(1,12,String(" "));
        }
        
        break;

        case START:
        if(blinkStatus) {
          screen->write(1,13,String((char) 0b00011110));
        } else {
          screen->write(1,13,String(" "));
        }
        break;

      }
      switch(counters->getDirection(3)) {
        case STOP:
        if(blinkStatus) {
          screen->write(1,17,String((char) 0b00011111));
        } else {
          screen->write(1,17,String(" "));
        }
        
        break;

        case START:
        if(blinkStatus) {
          screen->write(1,18,String((char) 0b00011110));
        } else {
          screen->write(1,18,String(" "));
        }
        break;
      }
    break;
    case SETTINGS:
    break;
  }
  screen->update();
}

void generateTone(int toneDelay) {
  tone(BUZZER_PIN, BUZZER_FREQUENCY, toneDelay);

}
void generate_alarm(int toneDelay) {
  tone(BUZZER_PIN, BUZZER_FREQUENCY, toneDelay);
}
