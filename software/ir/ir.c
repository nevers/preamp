#include "ir.h"
#include <p18cxxx.h>
#include <capture.h>
#include <timers.h>

void resetIR(void);
void invertEdge(void);
void updateEdge(void);

typedef struct {
    unsigned int edge : 1;
    unsigned char bitcount;
    unsigned long data;
} Capture;

typedef enum NEC_STATE {
    inactive,
    invalid,
    lead_pulse,
    lead_space,
    repeat_pulse,
    data_pulse,
    data_space,
    frame_end,
    timeout
} NecState;

NecState necState = inactive;
unsigned int withinFrame = 0;
Capture capture = {0};
Ir ir = {0};

void initIR(void) {
    TRISCbits.TRISC2 = 1; //Set pin 2 of PORTC to input as it used for capturing
    INTCON = 0; //Disable all interrupts

    // Configure timer 1 - It is used for demodulating the NEC command and address.
    OpenTimer1(TIMER_INT_ON & T1_16BIT_RW & T1_SOURCE_INT & T1_PS_1_1 & T1_SOURCE_CCP);

    // Configure timer 3
    OpenTimer3(TIMER_INT_ON & T3_16BIT_RW & T3_SOURCE_INT & T3_PS_1_8);

    // Disable all timers
    T1CONbits.TMR1ON = 0;
    T3CONbits.TMR3ON = 0;

    // Configure interrupts
    PIE1bits.TMR1IE = 1;
    PIE2bits.TMR3IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1; //Enable global interrupts

    resetIR();
    disableIR();
}

void resetIR(void) {
    necState = inactive;
    ir.decoded = 0;
    ir.repeated = 0;
    capture.edge = 0;
    capture.bitcount = 0;
    capture.data = 1;
    updateEdge();
    WriteTimer1(0);
}

void updateEdge(void) {
    if (capture.edge)
        OpenCapture1(CAPTURE_INT_ON & C1_EVERY_RISE_EDGE);
    else
        OpenCapture1(CAPTURE_INT_ON & C1_EVERY_FALL_EDGE);
}

void invertEdge(void) {
    capture.edge ^= 1;
    updateEdge();
}

void enableIR(void) {
    resetIR();
    PIE1bits.CCP1IE = 1;
}

void disableIR(void) {
    PIE1bits.CCP1IE = 0;
    T1CONbits.TMR1ON = 0;
    T3CONbits.TMR3ON = 0;
}

void timeoutIRBit(void) {
    if (necState == frame_end) {
        disableIR(); // Wait for the caller to re-enable IR
        ir.decoded = 1;
        return;
    }
    resetIR();
}

void timeoutIRFrame(void) {
    withinFrame = 0;
    T3CONbits.TMR3ON = 0;
}

void decodeIR(void) {
    WriteTimer1(1);
    invertEdge();

    if (necState == invalid)
        return;

    if (necState == inactive) {
        necState = lead_pulse;
        T1CONbits.TMR1ON = 1;
        T3CONbits.TMR3ON = 1;
        return;
    }

    if (necState == lead_pulse) {
        if (CCPR1H > NEC_LEAD_PULSE_MIN || CCPR1H < NEC_LEAD_PULSE_MAX)
            necState = lead_space;
        else
            necState = invalid;
        return;
    }

    if (necState == lead_space) {
        if (CCPR1H > NEC_LEAD_SPACE_MIN && CCPR1H < NEC_LEAD_SPACE_MAX) {
            necState = data_pulse;
            withinFrame = 1;
        } else if (CCPR1H > NEC_REPEAT_SPACE_MIN && CCPR1H < NEC_REPEAT_SPACE_MAX)
            necState = repeat_pulse;
        else
            necState = invalid;
        return;
    }

    if (necState == repeat_pulse && withinFrame) {
        if (CCPR1H > NEC_SHORT_MIN && CCPR1H < NEC_SHORT_MAX) {
            ir.decoded = 1;
            ir.repeated = 1;
            WriteTimer3(0);
        } else {
            necState = invalid;
        }
        return;
    }

    if (necState == data_pulse) {
        if (CCPR1H > NEC_SHORT_MIN && CCPR1H < NEC_SHORT_MAX)
            necState = data_space;
        else
            necState = invalid;
        return;
    }

    if (necState == data_space) {
        // Received a 0
        if (CCPR1H > NEC_SHORT_MIN && CCPR1H < NEC_SHORT_MAX) {
            capture.data = capture.data >> 1;
            capture.bitcount++;
            necState = data_pulse;
        // Received a 1
        } else if (CCPR1H > NEC_LONG_MIN && CCPR1H < NEC_LONG_MAX) {
            capture.data = capture.data >> 1;
            capture.data |= 0x80000000;
            capture.bitcount++;
            necState = data_pulse;
        } else {
            necState = invalid;
        }

        if (capture.bitcount == 32) {
            ir.address = (unsigned int) (capture.data & 0xffff);
            if ((ir.address & 0xff) == (~ir.address >> 8))
                ir.address &= 0xff;
            ir.command = (unsigned char) (capture.data >> 24);
            necState = frame_end;
        }
    }
}
