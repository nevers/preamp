#ifndef __CHANNELS_H
#define	__CHANNELS_H

#define CHANNEL1_PIN TRISBbits.RB5
#define CHANNEL2_PIN TRISBbits.RB4
#define CHANNEL3_PIN TRISBbits.RB3
#define CHANNEL4_PIN TRISBbits.RB2
#define CHANNEL5_PIN TRISBbits.RB1
#define CHANNEL6_PIN TRISBbits.RB0
#define MASTER_PIN TRISCbits.RC4

#define CHANNEL1 PORTBbits.RB5
#define CHANNEL2 PORTBbits.RB4
#define CHANNEL3 PORTBbits.RB3
#define CHANNEL4 PORTBbits.RB2
#define CHANNEL5 PORTBbits.RB1
#define CHANNEL6 PORTBbits.RB0
#define MASTER PORTCbits.RC4

#define DEFAULT_CHANNEL 1

void initAudioChannels(void);
void incrementChannel(void);
void decrementChannel(void);
int getChannel(void);
void setChannel(int);
void enableMasterChannel(void);
void toggleMasterChannel(void);

#endif