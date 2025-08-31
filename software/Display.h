#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Arduino.h"
#include <ShiftLcd.h>

using namespace std;

class Display {
    private: 
    unsigned int lcd_en;
    unsigned int lcd_d7;
    unsigned int lcd_ser;
    unsigned int lcd_clk;
    unsigned int lcd_rows;
    unsigned int lcd_cols;
    unsigned int lcd_space;

    char** screen;

    ShiftLcd* lcd;

    public:
    Display(unsigned int LCD_EN, unsigned int LCD_D7, unsigned int LCD_SER, unsigned int LCD_CLK, unsigned int ROWS, unsigned int COLS, unsigned int MAX_COLS);
    ~Display();
    
    void clear();
    void update();
    void write(unsigned int row,unsigned int col, String string);

};


#endif 
