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
#define BUTTON_RESOLUTION         200

#define BUTTON_PRESSED_TONE       200

Display* screen;
Settings* options;
Sensor* light;
Relays* counters;

void control();
void update();
void generateTone(int toneDelay);
void generate_alarm(int toneDelay);

typedef enum {GENERAL,SETTINGS,VIEW1,VIEW2,VIEW3,VIEW4} view;

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
  
  attachInterrupt(0,setup_editor,LOW);
  options->loadSaved(SETTINGS_ADDRESS);

  Timer1.initialize((DISPLAY_REFRESH_RATE/2));
  Timer1.attachInterrupt(update);
  interrupts(); 
}

void loop() {

}

void setup_editor() {
  noInterrupts(); 
  detachInterrupt(0);
  currentView=SETTINGS;
  interrupts(); 

  screen->clear();
  screen->write(0,0,"Potenza carico 1");
  screen->write(1,0,String((char) 0b00111110)+" ");
  screen->write(1,2,String(options->getPower1()));
  
  while(!digitalRead(SELECT_BUTTON)) {
    if (!digitalRead(PLUS_BUTTON) && options->getPower1()<MAX_SETTINGS_BOUND) {
      options->setPower1(options->getPower1()+1);
      screen->write(1,2,String(options->getPower1()));
    }
    if (!digitalRead(MINUS_BUTTON) && options->getPower1()>0) {
      options->setPower1(options->getPower1()-1);
      screen->write(1,2,String(options->getPower1()));
    }
    delay(BUTTON_RESOLUTION);
  }
  delay(BUTTON_RESOLUTION);
  generateTone(BUTTON_PRESSED_TONE);

  screen->clear();
  screen->write(0,0,"Potenza carico 2");
  screen->write(1,0,String((char) 0b00111110)+" ");
  screen->write(1,2,String(options->getPower2()));
  
  while(!digitalRead(SELECT_BUTTON)) {
    if (!digitalRead(PLUS_BUTTON) && options->getPower2()<MAX_SETTINGS_BOUND) {
      options->setPower2(options->getPower2()+1);
      screen->write(1,2,String(options->getPower2()));
    }
    if (!digitalRead(MINUS_BUTTON) && options->getPower2()>0) {
      options->setPower2(options->getPower2()-1);
      screen->write(1,2,String(options->getPower2()));
    }
    delay(BUTTON_RESOLUTION);
  }
  delay(BUTTON_RESOLUTION);
  generateTone(BUTTON_PRESSED_TONE);

  screen->clear();
  screen->write(0,0,"Potenza carico 3");
  screen->write(1,0,String((char) 0b00111110)+" ");
  screen->write(1,2,String(options->getPower3()));
  
  while(!digitalRead(SELECT_BUTTON)) {
    if (!digitalRead(PLUS_BUTTON) && options->getPower3()<MAX_SETTINGS_BOUND) {
      options->setPower3(options->getPower3()+1);
      screen->write(1,2,String(options->getPower3()));
    }
    if (!digitalRead(MINUS_BUTTON) && options->getPower3()>0) {
      options->setPower3(options->getPower3()-1);
      screen->write(1,2,String(options->getPower3()));
    }
    delay(BUTTON_RESOLUTION);
  }
  delay(BUTTON_RESOLUTION);
  generateTone(BUTTON_PRESSED_TONE);

  screen->clear();
  screen->write(0,0,"Potenza carico 4");
  screen->write(1,0,String((char) 0b00111110)+" ");
  screen->write(1,2,String(options->getPower4()));
  
  while(!digitalRead(SELECT_BUTTON)) {
    if (!digitalRead(PLUS_BUTTON) && options->getPower4()<MAX_SETTINGS_BOUND) {
      options->setPower4(options->getPower4()+1);
      screen->write(1,2,String(options->getPower4()));
    }
    if (!digitalRead(MINUS_BUTTON) && options->getPower4()>0) {
      options->setPower4(options->getPower4()-1);
      screen->write(1,2,String(options->getPower4()));
    }
    delay(BUTTON_RESOLUTION);
  }
  delay(BUTTON_RESOLUTION);
  generateTone(BUTTON_PRESSED_TONE);

  screen->clear();
  screen->write(0,0,"Ritardo Accensione");
  screen->write(1,0,String((char) 0b00111110)+" ");
  screen->write(1,2,String(options->getTimerOn()));
  
  while(!digitalRead(SELECT_BUTTON)) {
    if (!digitalRead(PLUS_BUTTON) && options->getTimerOn()<MAX_SETTINGS_BOUND) {
      options->setTimerOn(options->getTimerOn()+1);
      screen->write(1,2,String(options->getTimerOn()));
    }
    if (!digitalRead(MINUS_BUTTON) && options->getTimerOn()>0) {
      options->setTimerOn(options->getTimerOn()-1);
      screen->write(1,2,String(options->getTimerOn()));
    }
    delay(BUTTON_RESOLUTION);
  }
  delay(BUTTON_RESOLUTION);
  generateTone(BUTTON_PRESSED_TONE);

  screen->clear();
  screen->write(0,0,"Ritardo Spegnimento");
  screen->write(1,0,String((char) 0b00111110)+" ");
  screen->write(1,2,String(options->getTimerOff()));
  
  while(!digitalRead(SELECT_BUTTON)) {
    if (!digitalRead(PLUS_BUTTON) && options->getTimerOff()<MAX_SETTINGS_BOUND) {
      options->setTimerOff(options->getTimerOff()+1);
      screen->write(1,2,String(options->getTimerOff()));
    }
    if (!digitalRead(MINUS_BUTTON) && options->getTimerOn()>0) {
      options->setTimerOff(options->getTimerOff()-1);
      screen->write(1,2,String(options->getTimerOff()));
    }
    delay(BUTTON_RESOLUTION);
  }
  delay(BUTTON_RESOLUTION);
  generateTone(BUTTON_PRESSED_TONE);

  screen->clear();
  screen->write(0,0,"Avvisi sonori");
  screen->write(1,0,String((char) 0b00111110)+" ");
  if (options->getBuzzer()) {
    screen->write(1,2,"ON");
  } else {
    screen->write(1,2,"OFF");
  }
  
  while(!digitalRead(SELECT_BUTTON)) {
    if (!digitalRead(PLUS_BUTTON)) {
      screen->write(1,2,"ON");
      options->setBuzzer(true);
    }
    if (!digitalRead(MINUS_BUTTON) && options->getTimerOn()>0) {
      screen->write(1,2,"OFF");
      options->setBuzzer(false);
    }
    delay(BUTTON_RESOLUTION);
  }
  delay(BUTTON_RESOLUTION);
  generateTone(BUTTON_PRESSED_TONE);
  options->store(SETTINGS_ADDRESS);

  noInterrupts(); 
  attachInterrupt(0,setup_editor,LOW);
  currentView=GENERAL;
  interrupts(); 
}

void update() {
  unsigned int currentPower=light->getCurrentPower();

  if (!digitalRead(PLUS_BUTTON)) {
    generateTone(BUTTON_PRESSED_TONE);
    switch(currentView) {
      case GENERAL:
        currentView=VIEW1;
      break;

      case VIEW1:
        currentView=VIEW2;
      break;

      case VIEW2:
        currentView=VIEW3;
      break;

      case VIEW3:
        currentView=VIEW4;
      break;

      default:
        currentView=GENERAL;
      break;
    }
  }

  if (secondCounter>0) {
    secondCounter--;
  } else {
    secondCounter=1000000/DISPLAY_REFRESH_RATE;
    blinkStatus=!blinkStatus;
  }
  
  switch (currentView) {
    case GENERAL:
      screen->write(0,0,"Potenza:");
      screen->write(0,10,String(currentPower)+"W");
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

    case VIEW1:
      screen->write(0,0,"Porta 1: ");
      if (!digitalRead(RELAY1_PIN)) {
        screen->write(0,10,"ON");
      } else {
        screen->write(0,10,"OFF");
      }
      switch(counters->getDirection(0)) {
        case STOP:
          screen->write(1,0,"Spegnimento in "+String(counters->getCount(0)));
        break;

        case START:
          screen->write(1,0,"Accensione in "+String(counters->getCount(0)));
        break;
      }
    break;

    case VIEW2:
      screen->write(0,0,"Porta 2: ");
      if (!digitalRead(RELAY2_PIN)) {
        screen->write(0,10,"ON");
      } else {
        screen->write(0,10,"OFF");
      }
      switch(counters->getDirection(1)) {
        case STOP:
          screen->write(1,0,"Spegnimento in "+String(counters->getCount(1)));
        break;

        case START:
          screen->write(1,0,"Accensione in "+String(counters->getCount(1)));
        break;
      }
    break;

    case VIEW3:
      screen->write(0,0,"Porta 3: ");
      if (!digitalRead(RELAY3_PIN)) {
        screen->write(0,10,"ON");
      } else {
        screen->write(0,10,"OFF");
      }
      switch(counters->getDirection(2)) {
        case STOP:
          screen->write(1,0,"Spegnimento in "+String(counters->getCount(2)));
        break;

        case START:
          screen->write(1,0,"Accensione in "+String(counters->getCount(2)));
        break;
      }
    break;

    case VIEW4:
      screen->write(0,0,"Porta 4: ");
      if (!digitalRead(RELAY4_PIN)) {
        screen->write(0,10,"ON");
      } else {
        screen->write(0,10,"OFF");
      }
      switch(counters->getDirection(3)) {
        case STOP:
          screen->write(1,0,"Spegnimento in "+String(counters->getCount(3)));
        break;

        case START:
          screen->write(1,0,"Accensione in "+String(counters->getCount(3)));
        break;
      }
    break;

    default:
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
