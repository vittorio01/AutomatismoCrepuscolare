#include "./Display/Display.h"
#include "./Settings/Settings.h"
#include "./Sensor/Sensor.h"

#define LCD_SHIFT_EN      4
#define LCD_SHIFT_D7      5
#define LCD_SHIFT_SER     6
#define LCD_SHIFT_CLK     7

#define LCD_FORMAT_ROWS   2
#define LCD_FORMAT_COLS   20
#define LCD_MAX_COLS      38
#define SETTINGS_ADDRESS  0

#define SENSOR_PIN        A0

Display display(LCD_SHIFT_EN, LCD_SHIFT_D7, LCD_SHIFT_SER, LCD_SHIFT_CLK,LCD_FORMAT_ROWS,LCD_FORMAT_COLS,LCD_MAX_COLS);
Settings settings();
Sensor sensor(SENSOR_PIN);

void setup() {
  display.update();
  settings.loadDefault(SETTINGS_ADDRESS);
}

void loop() {

}
