#include "Display.h"
//20x2 lcd, max 38 per line
#define LCD_SHIFT_EN    4
#define LCD_SHIFT_D7    5
#define LCD_SHIFT_SER   6
#define LCD_SHIFT_CLK   7

#define LCD_FORMAT_ROWS 2
#define LCD_FORMAT_COLS 38

Display display(LCD_SHIFT_EN, LCD_SHIFT_D7, LCD_SHIFT_SER, LCD_SHIFT_CLK,LCD_FORMAT_ROWS,LCD_FORMAT_COLS);

void setup() {
  display.screen[0]="ciao";
  display.update();
}

void loop() {

}
