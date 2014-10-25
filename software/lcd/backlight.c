#include "xlcd.h"
#include <p18cxxx.h>

unsigned long pollCount = 0;

void checkBacklightXLCDTimer() {
    if (isBacklightXLCD())
        pollCount++;

    if (pollCount > BACKLIGHT_TIMEOUT)
        disableBacklightXLCD();
}

void enableBacklightXLCD() {
    pollCount = 0;
    LATCbits.LATC1 = 1;
}

void disableBacklightXLCD() {
    pollCount = 0;
    LATCbits.LATC1 = 0;
}

int isBacklightXLCD() {
    return LATCbits.LATC1;
}