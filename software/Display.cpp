#include "Display.h"
#include "Arduino.h"

using namespace std;

Display::Display(unsigned int LCD_EN, unsigned int LCD_D7, unsigned int LCD_SER, unsigned int LCD_CLK, unsigned int ROWS, unsigned int COLS) {
    lcd_en=LCD_EN;
    lcd_d7=LCD_D7;
    lcd_ser=LCD_SER;
    lcd_clk=LCD_CLK;
    lcd_rows=ROWS;
    lcd_cols=COLS;
    if (ROWS>0) {
        screen= new char*[ROWS];
        for (int row=0;row<ROWS;row++) {
            screen[row]=new char[lcd_cols+1];
        }
    } else {
        screen=NULL;
    }
    lcd=new ShiftLcd(lcd_en, lcd_d7, lcd_ser, lcd_clk);
    lcd->begin();
    clear();
    
}
Display::~Display() {
    if (screen!=NULL) {
        for (int row=0;row<lcd_rows;row++) {
            delete [] screen[row];
        }
        delete [] screen;
    }
    delete [] lcd;

}

void Display::clear() {
    lcd->clear();
    for (int row=0;row<lcd_rows;row++) {
        for (int col=0;col<lcd_cols-1;col++) {
            screen[row][col]=' ';
        }
        screen[row][lcd_cols-1]='\n';
    }
}
void Display::update() {
    if (screen==NULL) return;
    if (lcd_rows<2) {
        lcd->write(screen[0]);
    } else {
        lcd->write(screen[0],screen[1]);
    }
}
    
