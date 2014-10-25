#ifndef __VOLUME_H
#define	__VOLUME_H

#define VOL_CS_TRIS TRISCbits.RC0
#define VOL_SCK_TRIS TRISCbits.RC3
#define VOL_SDO_TRIS TRISCbits.RC5

#define VOL_CS LATCbits.LATC0 //CS PGA2311

#define DEFAULT_VOLUME 92
#define STEP_SIZE 2

void initVolume(void);
void incrementVolume(void);
void decrementVolume(void);
void mute(void);
int getVolume(void);
int getVolumeInDB(void);
int getVolumeInBars(void);

#endif

