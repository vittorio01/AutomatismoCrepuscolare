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

typedef enum {CONTROL,MINUS,PLUS,NONE} button;
typedef enum {GENERAL,SETTINGS,VIEW1,VIEW2,VIEW3,VIEW4} view;

Display* screen;
Settings* options;
Sensor* light;
Relays* counters;

void control();
void update();
void generateTone(int toneDelay);
void generate_alarm(int toneDelay);
button waitButton();

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

button waitButton() {
  bool select=false;
  bool plus=false;
  bool minus=false;
  while(!select && !plus && !minus) {
    delay(BUTTON_RESOLUTION);
    select=!digitalRead(SELECT_BUTTON);
    plus=!digitalRead(PLUS_BUTTON);
    minus=!digitalRead(MINUS_BUTTON);
  }
  if (select) return CONTROL;
  if (plus) return PLUS;
  if (minus) return MINUS;
}

void setup_editor() {
  noInterrupts(); 
  detachInterrupt(0);
  currentView=SETTINGS;
  interrupts(); 

  button lastButton;

  screen->clear();
  screen->write(0,0,"Potenza carichi");
  if (waitButton()==PLUS) {
    lastButton=NONE;
    screen->clear();
    screen->write(0,0,"Potenza carico 1");
    screen->write(1,0,String((char) 0b00111110)+" ");
    screen->write(1,2,String(options->getPower(0))+"    ");
    
    while(lastButton!=CONTROL) {
      switch (lastButton) {
        case PLUS:
          options->setPower(0,options->getPower(0)+1);
          screen->write(1,2,String(options->getPower(0))+"    ");
        break;
        case MINUS:
          options->setPower(0,options->getPower(0)-1);
          screen->write(1,2,String(options->getPower(0))+"    ");
      }
      lastButton=waitButton();
    }
    generateTone(BUTTON_PRESSED_TONE);
    
    lastButton=NONE;
    screen->clear();
    screen->write(0,0,"Potenza carico 2");
    screen->write(1,0,String((char) 0b00111110)+" ");
    screen->write(1,2,String(options->getPower(1))+"    ");
    
    while(lastButton!=CONTROL) {
      switch (lastButton) {
        case PLUS:
          options->setPower(1,options->getPower(1)+1);
          screen->write(1,2,String(options->getPower(1))+"    ");
        break;
        case MINUS:
          options->setPower(1,options->getPower(1)-1);
          screen->write(1,2,String(options->getPower(1))+"    ");
      }
      lastButton=waitButton();
    }
    generateTone(BUTTON_PRESSED_TONE);

    lastButton=NONE;
    screen->clear();
    screen->write(0,0,"Potenza carico 3");
    screen->write(1,0,String((char) 0b00111110)+" ");
    screen->write(1,2,String(options->getPower(2))+"    ");
    
    while(lastButton!=CONTROL) {
      switch (lastButton) {
        case PLUS:
          options->setPower(2,options->getPower(2)+1);
          screen->write(1,2,String(options->getPower(2))+"    ");
        break;
        case MINUS:
          options->setPower(2,options->getPower(2)-1);
          screen->write(1,2,String(options->getPower(2))+"    ");
      }
      lastButton=waitButton();
    }
    generateTone(BUTTON_PRESSED_TONE);

    lastButton=NONE;
    screen->clear();
    screen->write(0,0,"Potenza carico 4");
    screen->write(1,0,String((char) 0b00111110)+" ");
    screen->write(1,2,String(options->getPower(3))+"    ");
    
    while(lastButton!=CONTROL) {
      switch (lastButton) {
        case PLUS:
          options->setPower(3,options->getPower(3)+1);
          screen->write(1,2,String(options->getPower(3))+"    ");
        break;
        case MINUS:
          options->setPower(3,options->getPower(2)-1);
          screen->write(1,2,String(options->getPower(3))+"    ");
      }
      lastButton=waitButton();
    }
    generateTone(BUTTON_PRESSED_TONE);
  }

  screen->clear();
  screen->write(0,0,"Maschere");
  if (waitButton()==PLUS) {
    lastButton=NONE;
    screen->clear();
    screen->write(0,0,"Maschera carico 1");
    screen->write(1,0,String((char) 0b00111110)+" ");
    if (options->getMask(0)) {
      screen->write(1,2,"ON ");
    } else {
      screen->write(1,2,"OFF");
    }
    while(lastButton!=CONTROL) {
      switch (lastButton) {
        case PLUS:
          options->setMask(0,true);
          screen->write(1,2,"ON ");
        break;
        case MINUS:
          options->setMask(0,false);
          screen->write(1,2,"OFF ");
      }
      lastButton=waitButton();
    }
    generateTone(BUTTON_PRESSED_TONE);
    
    lastButton=NONE;
    screen->clear();
    screen->write(0,0,"Maschera carico 2");
    screen->write(1,0,String((char) 0b00111110)+" ");
    if (options->getMask(1)) {
      screen->write(1,2,"ON ");
    } else {
      screen->write(1,2,"OFF");
    }
    while(lastButton!=CONTROL) {
      switch (lastButton) {
        case PLUS:
          options->setMask(1,true);
          screen->write(1,2,"ON ");
        break;
        case MINUS:
          options->setMask(1,false);
          screen->write(1,2,"OFF ");
      }
      lastButton=waitButton();
    }
    generateTone(BUTTON_PRESSED_TONE);

    lastButton=NONE;
    screen->clear();
    screen->write(0,0,"Maschera carico 3");
    screen->write(1,0,String((char) 0b00111110)+" ");
    if (options->getMask(2)) {
      screen->write(1,2,"ON ");
    } else {
      screen->write(1,2,"OFF");
    }
    while(lastButton!=CONTROL) {
      switch (lastButton) {
        case PLUS:
          options->setMask(2,true);
          screen->write(1,2,"ON ");
        break;
        case MINUS:
          options->setMask(2,false);
          screen->write(1,2,"OFF ");
      }
      lastButton=waitButton();
    }
    generateTone(BUTTON_PRESSED_TONE);

    lastButton=NONE;
    screen->clear();
    screen->write(0,0,"Maschera carico 4");
    screen->write(1,0,String((char) 0b00111110)+" ");
    if (options->getMask(3)) {
      screen->write(1,2,"ON ");
    } else {
      screen->write(1,2,"OFF");
    }
    while(lastButton!=CONTROL) {
      switch (lastButton) {
        case PLUS:
          options->setMask(3,true);
          screen->write(1,2,"ON ");
        break;
        case MINUS:
          options->setMask(3,false);
          screen->write(1,2,"OFF ");
      }
      lastButton=waitButton();
    }
    generateTone(BUTTON_PRESSED_TONE);
  }


  lastButton=NONE;
  screen->clear();
  screen->write(0,0,"Ritardo accensioni");
  screen->write(1,0,String((char) 0b00111110)+" ");
  screen->write(1,2,String(options->getTimerOn())+"    ");
  
  while(lastButton!=CONTROL) {
    switch (lastButton) {
      case PLUS:
        options->setTimerOn(options->getTimerOn()+1);
        screen->write(1,2,String(options->getTimerOn())+"    ");
      break;
      case MINUS:
        options->setTimerOn(options->getTimerOn()-1);
        screen->write(1,2,String(options->getTimerOn())+"    ");
    }
    lastButton=waitButton();
  }
  generateTone(BUTTON_PRESSED_TONE);

  lastButton=NONE;
  screen->clear();
  screen->write(0,0,"Ritardo spegnimenti");
  screen->write(1,0,String((char) 0b00111110)+" ");
  screen->write(1,2,String(options->getTimerOff())+"    ");
  
  while(lastButton!=CONTROL) {
    switch (lastButton) {
      case PLUS:
        options->setTimerOff(options->getTimerOff()+1);
        screen->write(1,2,String(options->getTimerOff())+"    ");
      break;
      case MINUS:
        options->setTimerOff(options->getTimerOff()-1);
        screen->write(1,2,String(options->getTimerOff())+"    ");
    }
    lastButton=waitButton();
  }
  generateTone(BUTTON_PRESSED_TONE);

  lastButton=NONE;
  screen->clear();
  screen->write(0,0,"Avvisi Sonori");
  screen->write(1,0,String((char) 0b00111110)+" ");
  if (options->getBuzzer()) {
    screen->write(1,2,"ON ");
  } else {
    screen->write(1,2,"OFF");
  }
  while(lastButton!=CONTROL) {
    switch (lastButton) {
      case PLUS:
        options->setBuzzer(true);
        screen->write(1,2,"ON ");
      break;
      case MINUS:
        options->setBuzzer(false);
        screen->write(1,2,"OFF ");
    }
    lastButton=waitButton();
  }
  generateTone(BUTTON_PRESSED_TONE);

  noInterrupts(); 
  options->store(SETTINGS_ADDRESS);
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
    counters->updateStatus();

    int leftPower=currentPower;
    for (int l=0;l<LOADS_NUMBER;l++) {
      if (!options->getMask(l)) {
        leftPower-=options->getPower(l);
        switch (counters->getDirection(l)) {
          case STOP:
            if (leftPower>0) counters->setCount(l,0,ON);
          break;
          case START:
            if (leftPower<0) counters->setCount(l,0,OFF);
          break;
          case ON:
            if (leftPower<0) counters->setCount(l,options->getTimerOff(),STOP);
          break;
          case OFF:
            if (leftPower>0) counters->setCount(l,options->getTimerOn(),START);
          break;
        }
      } else {
        counters->setCount(l,0,OFF);
      }
    }

    if (counters->getDirection(0)==START || counters->getDirection(0)==OFF) digitalWrite(RELAY1_PIN,false);
    else digitalWrite(RELAY1_PIN,true);
    if (counters->getDirection(1)==START || counters->getDirection(1)==OFF) digitalWrite(RELAY2_PIN,false);
    else digitalWrite(RELAY2_PIN,true);
    if (counters->getDirection(2)==START || counters->getDirection(2)==OFF) digitalWrite(RELAY3_PIN,false);
    else digitalWrite(RELAY3_PIN,true);
    if (counters->getDirection(3)==START || counters->getDirection(3)==OFF) digitalWrite(RELAY4_PIN,false);
    else digitalWrite(RELAY4_PIN,true);
  }
  
  switch (currentView) {
    case GENERAL:
      screen->write(0,0,"Potenza:");
      screen->write(0,10,String(currentPower)+"W    ");
      if (!digitalRead(RELAY1_PIN)) {
        screen->write(1,0,"ON ");
      } else {
        screen->write(1,0,"OFF");
      }
      if (!digitalRead(RELAY2_PIN)) {
        screen->write(1,5,"ON ");
      } else {
        screen->write(1,5,"OFF");
      }
      if (!digitalRead(RELAY1_PIN)) {
        screen->write(1,10,"ON ");
      } else {
        screen->write(1,10,"OFF");
      }
      if (!digitalRead(RELAY1_PIN)) {
        screen->write(1,15,"ON ");
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
      screen->write(0,0,"Porta 1:            ");
      if (!digitalRead(RELAY1_PIN)) {
        screen->write(0,10,"ON ");
      } else {
        screen->write(0,10,"OFF");
      }
      switch(counters->getDirection(0)) {
        case STOP:
          screen->write(1,0,"Spegnimento in "+String(counters->getCount(0))+"    ");
        break;

        case START:
          screen->write(1,0,"Accensione in "+String(counters->getCount(0))+"    ");
        break;

        default:
          if (options->getMask(0)) {
            screen->write(1,0,"Mascherato          ");
          } else {
            screen->write(1,0,"                    ");
          }
          
        break;
      }
    break;

    case VIEW2:
      screen->write(0,0,"Porta 2:            ");
      if (!digitalRead(RELAY2_PIN)) {
        screen->write(0,10,"ON ");
      } else {
        screen->write(0,10,"OFF");
      }
      switch(counters->getDirection(1)) {
        case STOP:
          screen->write(1,0,"Spegnimento in "+String(counters->getCount(1))+"    ");
        break;

        case START:
          screen->write(1,0,"Accensione in "+String(counters->getCount(1))+"    ");
        break;
        default:
          if (options->getMask(1)) {
            screen->write(1,0,"Mascherato          ");
          } else {
            screen->write(1,0,"                    ");
          }
          
        break;
      }
    break;

    case VIEW3:
      screen->write(0,0,"Porta 3:            ");
      if (!digitalRead(RELAY3_PIN)) {
        screen->write(0,10,"ON ");
      } else {
        screen->write(0,10,"OFF");
      }
      switch(counters->getDirection(2)) {
        case STOP:
          screen->write(1,0,"Spegnimento in "+String(counters->getCount(2))+"    ");
        break;

        case START:
          screen->write(1,0,"Accensione in "+String(counters->getCount(2))+"    ");
        break;

        default:
          if (options->getMask(2)) {
            screen->write(1,0,"Mascherato          ");
          } else {
            screen->write(1,0,"                    ");
          }
          
        break;
      }
    break;

    case VIEW4:
      screen->write(0,0,"Porta 4:            ");
      if (!digitalRead(RELAY4_PIN)) {
        screen->write(0,10,"ON ");
      } else {
        screen->write(0,10,"OFF");
      }
      switch(counters->getDirection(3)) {
        case STOP:
          screen->write(1,0,"Spegnimento in "+String(counters->getCount(3))+"    ");
        break;

        case START:
          screen->write(1,0,"Accensione in "+String(counters->getCount(3))+"    ");
        break;

        default:
          if (options->getMask(3)) {
            screen->write(1,0,"Mascherato          ");
          } else {
            screen->write(1,0,"                    ");
          }
          
        break;
      }
    break;

    default:
    break;
  }
  screen->update();
}

void generateTone(int toneDelay) {
  if (options->getBuzzer()) tone(BUZZER_PIN, BUZZER_FREQUENCY, toneDelay);

}
void generate_alarm(int toneDelay) {
  if (options->getBuzzer()) tone(BUZZER_PIN, BUZZER_FREQUENCY, toneDelay);
}
