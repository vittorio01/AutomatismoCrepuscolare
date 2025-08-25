#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "Arduino.h"
#include <ShiftLcd.h>

using namespace std;

class Display {
    public: 
    unsigned int lcd_en;
    unsigned int lcd_d7;
    unsigned int lcd_ser;
    unsigned int lcd_clk;
    unsigned int lcd_rows;
    unsigned int lcd_cols;

    char** screen;

    ShiftLcd* lcd;

    Display(unsigned int LCD_EN, unsigned int LCD_D7, unsigned int LCD_SER, unsigned int LCD_CLK, unsigned int ROWS, unsigned int COLS);
    ~Display();
    
    void clear();
    void update();
    
};


#endif 
