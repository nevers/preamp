#include <p18F4520.h>
#include <stdlib.h>
#include <delays.h>
#include <spi.h>
#include <string.h>

#include "lcd/xlcd.h"
#include "ir/ir.h"
#include "volume/volume.h"
#include "channels/channels.h"
#include "encoder/encoder.h"

void interruptHandler(void);
void init(void);
void displayCredits(void);
void checkDecodedIRCommands(void);
void checkEncoderState(void);
void updateDisplay(void);
void clearDisplay(void);
void displayAudioChannel(void);
void displayVolumeInDB(void);
void displayVolumeInBars(void);

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PWRT = OFF
#pragma config DEBUG = OFF
#pragma config BOREN = NOSLP
#pragma config BORV = 1
#pragma config IESO = OFF

#pragma code interrupt_vector = 0x08

void atInterrupt(void) {
    _asm goto interruptHandler _endasm
}

#pragma code
#pragma interrupt interruptHandler

void interruptHandler(void) {
    //An overflow occured on timer 1.
    if (PIR1bits.TMR1IF) { 
        timeoutIRBit();
        PIR1bits.TMR1IF = 0;
    }

    // An overflow occured on timer 3.
    if (PIR2bits.TMR3IF) {
        timeoutIRFrame();
        PIR2bits.TMR3IF = 0;
    }

    // An interrupt occured on CCP1 (RC2 pin)
    if (PIR1bits.CCP1IF) { 
        decodeIR();
        PIR1bits.CCP1IF = 0;
    }
}

void main(void) {
    init();
    enableBacklightXLCD();
    enableIR();
    displayCredits();
    enableMasterChannel();
    setChannel(DEFAULT_CHANNEL);
    updateDisplay();

    while (1) {
        checkDecodedIRCommands();
        checkEncoderState();
        checkBacklightXLCDTimer();
    }
}

void init(void) {
    ADCON1 = 0xff; //Set all analog pins as digital on PORTA
    CMCON = 0xff; // Disable all comparators on PORTA
    initIR();
    initXLCD();
    initVolume();
    initAudioChannels();
    initEncoder();
}

void displayCredits() {
    clearDisplay();

    while (BusyXLCD());
    putrsXLCD("       The Preamp");

    while (BusyXLCD());
    SetDDRamAddr(40); // Set cursor on second line

    Delay10KTCYx(255);

    while (BusyXLCD());
    putrsXLCD("Nick Evers   ");

    while (BusyXLCD());
    putrsXLCD(__DATE__);

    Delay10KTCYx(255);
    Delay10KTCYx(255);
    Delay10KTCYx(255);
}

void checkDecodedIRCommands() {
    if (!ir.decoded)
        return;

    if (ir.repeated && ir.command != IR_CMD_VOL_UP && ir.command != IR_CMD_VOL_DOWN)
        return;

    if (ir.address != 27526) {
        enableIR();
        return;
    }

    switch (ir.command) {
        case IR_CMD_MUTE:
            mute();
            break;
        case IR_CMD_VOL_UP:
            incrementVolume();
            break;
        case IR_CMD_VOL_DOWN:
            decrementVolume();
            break;
        case IR_CMD_CH_UP:
            incrementChannel();
            break;
        case IR_CMD_CH_DOWN:
            decrementChannel();
            break;
        case IR_CMD_1:
            setChannel(1);
            break;
        case IR_CMD_2:
            setChannel(2);
            break;
        case IR_CMD_3:
            setChannel(3);
            break;
        case IR_CMD_4:
            setChannel(4);
            break;
        case IR_CMD_5:
            setChannel(5);
            break;
        case IR_CMD_6:
            setChannel(6);
            break;
    }

    updateDisplay();
    enableIR();
}

void checkEncoderState() {
    EncoderState encoderState = getEncoderState();

    switch (encoderState) {
        case cw:
            incrementVolume();
            break;
        case ccw:
            decrementVolume();
            break;
        case pressed:
            incrementChannel();
            break;
    }

    if (encoderState != inactive)
        updateDisplay();
    clearEncoderState();
}

void updateDisplay() {
    clearDisplay();
    displayAudioChannel();
    displayVolumeInDB();
    displayVolumeInBars();
    enableBacklightXLCD();
}

void clearDisplay() {
    while (BusyXLCD());
    WriteCmdXLCD(CLEAR_DISPLAY);

    while (BusyXLCD());
    SetDDRamAddr(0);
}

void displayAudioChannel() {
    char buffer[] = " ";

    while (BusyXLCD());
    putrsXLCD("Channel: ");

    itoa(getChannel(), buffer);
    while (BusyXLCD());
    putsXLCD(buffer);
}

void displayVolumeInDB() {
    char buffer[] = "    ";
    int volumeInDb = getVolumeInDB();

    while (BusyXLCD());
    if (volumeInDb == 0)
        SetDDRamAddr(21);
    else if (volumeInDb > 9 || volumeInDb < -9)
        SetDDRamAddr(19);
    else
        SetDDRamAddr(20);

    if (volumeInDb > 0) {
        while (BusyXLCD());
        putrsXLCD("+");
    }

    itoa(volumeInDb, buffer);
    while (BusyXLCD());
    putsXLCD(buffer);

    while (BusyXLCD());
    putrsXLCD("dB");
}

void displayVolumeInBars() {
    int volumeInBars = getVolumeInBars();
    int i;

    while (BusyXLCD());
    SetDDRamAddr(40); // Set cursor on second line

    for (i = 0; i < volumeInBars; i++) {
        while (BusyXLCD());
        putcXLCD(0xff);
    }
}