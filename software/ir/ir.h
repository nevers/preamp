#ifndef __IR_H
#define	__IR_H

void initIR(void);
void enableIR(void);
void disableIR(void);
void decodeIR(void);
void timeoutIRBit(void);
void timeoutIRFrame(void);

typedef struct {
    unsigned int decoded : 1;
    unsigned int repeated : 1;
    unsigned int address;
    unsigned char command;
} Ir;

extern Ir ir;


/*
 * NEC: Pulse distance encoding
 *
 * First pulse : 9ms 4.5ms
 * 0 : 560us 560us
 * 1 : 560us 1690us
 *
 * Address (8 bits) is sent first, then ~Address
 * Command (8 bits) follows, then ~Command
 * LSB is sent first
 *
 * NEC extended allows 16 bits addresses. In this case, the 8 bits following
 * Address are not ~Address, but the address extension.
 */

#define	NEC_LEAD_PULSE_MIN 68
#define	NEC_LEAD_PULSE_MAX 72
#define	NEC_LEAD_SPACE_MIN 32
#define	NEC_LEAD_SPACE_MAX 36
#define NEC_REPEAT_SPACE_MIN 15
#define NEC_REPEAT_SPACE_MAX 19
#define	NEC_SHORT_MIN 2
#define	NEC_SHORT_MAX 5
#define	NEC_LONG_MIN 11
#define	NEC_LONG_MAX 13
#define	NEC_TIMEOUT 100

#define IR_CMD_POWER 237
#define IR_CMD_PC_TV 227
#define IR_CMD_MODE  238
#define IR_CMD_PREVIEW 240
#define IR_CMD_MUTE 236
#define IR_CMD_TXT 239
#define IR_CMD_1 254
#define IR_CMD_2 253
#define IR_CMD_3 252
#define IR_CMD_4 251
#define IR_CMD_5 250
#define IR_CMD_6 249
#define IR_CMD_7 248
#define IR_CMD_8 247
#define IR_CMD_9 246
#define IR_CMD_0 255
#define IR_CMD_SNAP 245
#define IR_CMD_100 232
#define IR_CMD_CH_UP 229
#define IR_CMD_CH_DOWN 225
#define IR_CMD_VOL_UP 228
#define IR_CMD_VOL_DOWN 224
#define IR_CMD_PAUSE 242
#define IR_CMD_RECORD 241
#define IR_CMD_STOP	226
#define IR_CMD_PLAY 230
#define IR_CMD_REPLAY 233
#define IR_CMD_SHIFT 235
#define IR_CMD_FFWD 243
#define IR_CMD_FREV 231

#endif
