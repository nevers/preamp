#include "xlcd.h"
#include <p18cxxx.h>
#include <delays.h>

void initXLCD() {
    BACKLIGHT_PORT = 0;
    TRIS_DATA_PORT = 0;

    OpenXLCD(EIGHT_BIT & LINES_5X7);
    
    while (BusyXLCD());
    WriteCmdXLCD(CURSOR_OFF & BLINK_OFF);
}

void DelayXLCD(void) {
    Delay1KTCYx(10);
}

void DelayPORXLCD(void) {
    Delay1KTCYx(30);
}

void DelayFor18TCY(void) {
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
}