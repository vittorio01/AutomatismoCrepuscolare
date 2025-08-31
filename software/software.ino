#include "Display.h"
#include "Settings.h"
#include "Sensor.h"
#include "Relays.h"

#include <TimerOne.h>

#define LCD_SHIFT_EN      D4
#define LCD_SHIFT_D7      D5
#define LCD_SHIFT_SER     D6
#define LCD_SHIFT_CLK     D7

#define LCD_FORMAT_ROWS   2
#define LCD_FORMAT_COLS   20
#define LCD_MAX_COLS      38
#define SETTINGS_ADDRESS  0

#define PHOTO_PIN         A0
#define PHOTO_BASEVALUE   0
#define PHOTO_INCREMENT   10

#define SELECT_BUTTON     D2
#define MINUS_BUTTON      D8
#define PLUS_BUTTON       D9

#define RELAY1_PIN        D0
#define RELAY2_PIN        D1
#define RELAY3_PIN        D11
#define RELAY4_PIN        D10

#define BUZZER_PIN        D12
#define BUZZER_FREQUENCY  3500

Display* screen;
Settings* options;
Sensor* light;

void control();
void update();
void generateTone(int toneDelay);
void generate_alarm(int toneDelay);

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

  screen=new Display(LCD_SHIFT_EN, LCD_SHIFT_D7, LCD_SHIFT_SER, LCD_SHIFT_CLK,LCD_FORMAT_ROWS,LCD_FORMAT_COLS,LCD_MAX_COLS);
  options=new Settings();
  light=new Sensor(PHOTO_PIN,PHOTO_BASEVALUE,PHOTO_INCREMENT);
  
  options->loadSaved(SETTINGS_ADDRESS);

    
  attachInterrupt(0, control,LOW);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(update);
  interrupts(); 
}

void loop() {

}

void control() {

}

void update() {

}

void generateTone(int toneDelay) {
  tone(BUZZER_PIN, BUZZER_FREQUENCY, toneDelay);

}
void generate_alarm(int toneDelay) {
  tone(BUZZER_PIN, BUZZER_FREQUENCY, toneDelay);
}