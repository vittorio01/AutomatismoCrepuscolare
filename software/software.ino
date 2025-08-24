#include <ShiftLcd.h>
//20x2 lcd, max 38 per line
int LCD_EN = 4;
int LCD_D7 = 5; 
int SHIFT_SER = 6; 
int SHIFT_SRCLK = 7;

ShiftLcd lcd(LCD_EN, LCD_D7, SHIFT_SER, SHIFT_SRCLK);

void setup() {
  lcd.begin();
  lcd.write("Hello, World←000000000000000000000");
}

void loop() {
  lcd.scrollLeft(100);
}
