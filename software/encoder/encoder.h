#ifndef __ENCODER_H
#define	__ENCODER_H

#define ENCODER_A_PIN  TRISAbits.RA3
#define ENCODER_B_PIN  TRISAbits.RA4
#define ENCODER_SW_PIN TRISAbits.RA5

#define ENCODER_A  PORTAbits.RA3
#define ENCODER_B  PORTAbits.RA4
#define ENCODER_BUTTON PORTAbits.RA5

#define DEBOUNCE_DELAY 50

typedef enum ENCODER_STATE {
    inactive,
    cw,
    ccw,
    pressed
} EncoderState;

void initEncoder(void);
void clearEncoderState(void);
EncoderState getEncoderState(void);

#endif