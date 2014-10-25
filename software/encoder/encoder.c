#include "encoder.h"
#include <p18cxxx.h>
#include <delays.h>


int isEncoderAChanged(void);
int isEncoderBChanged(void);
int isEncoderButtonChanged(void);
void storeEncoders(void);

EncoderState encoderState = inactive;

int encoderA = 0;
int encoderB = 0;
int encoderButton = 0;
int prevEncoderA = 0;
int prevEncoderB = 0;
int prevEncoderButton = 0;

void initEncoder() {
    ENCODER_A_PIN = 1;
    ENCODER_B_PIN = 1;
    ENCODER_SW_PIN = 1;
}

void clearEncoderState() {
    encoderState = inactive;
}

EncoderState getEncoderState() {
    if (isEncoderAChanged() || isEncoderBChanged())
        if (encoderA && encoderB && !prevEncoderB)
            encoderState = ccw;
        else if (encoderA && !encoderB && prevEncoderB)
            encoderState = cw;
    if (isEncoderButtonChanged() && encoderButton)
        encoderState = pressed;
    storeEncoders();
    return encoderState;
}

int isEncoderAChanged() {
    int encoderASample1;
    int encoderASample2;

    encoderASample1 = !ENCODER_A;
    Delay10TCYx(DEBOUNCE_DELAY);
    encoderASample2 = !ENCODER_A;

    if (encoderASample1 == encoderASample2)
        encoderA = encoderASample1;
    else
        return 0;
    return encoderA != prevEncoderA;
}

int isEncoderBChanged() {
    int encoderBSample1;
    int encoderBSample2;

    encoderBSample1 = !ENCODER_B;
    Delay10TCYx(DEBOUNCE_DELAY);
    encoderBSample2 = !ENCODER_B;

    if (encoderBSample1 == encoderBSample2)
        encoderB = encoderBSample1;
    else
        return 0;
    return encoderB != prevEncoderB;
}

int isEncoderButtonChanged() {
    int encoderButtonSample1;
    int encoderButtonSample2;

    encoderButtonSample1 = !ENCODER_BUTTON;
    Delay10TCYx(DEBOUNCE_DELAY);
    encoderButtonSample2 = !ENCODER_BUTTON;

    if (encoderButtonSample1 == encoderButtonSample2)
        encoderButton = encoderButtonSample1;
    else
        return 0;
    return encoderButton != prevEncoderButton;
}

void storeEncoders() {
    prevEncoderA = encoderA;
    prevEncoderB = encoderB;
    prevEncoderButton = encoderButton;
}