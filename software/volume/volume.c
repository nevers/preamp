#include "volume.h"
#include <p18cxxx.h>
#include <spi.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int numerator;
    int denuminator;
} Fraction;

typedef struct {
    int result;
    int remainder;
} Division;

void setVolume(int);
Division divide(Fraction);

int volume = DEFAULT_VOLUME;
int volumeBeforeMute = 0;

void initVolume() {
    VOL_CS_TRIS = 0;
    VOL_SCK_TRIS = 0;
    VOL_SDO_TRIS = 0;
    VOL_CS = 1; //CS of PGA2311
    OpenSPI(SPI_FOSC_16, MODE_00, SMPEND);
    setVolume(DEFAULT_VOLUME);
}

int getVolume() {
    return volume;
}

void incrementVolume() {
    int newVolume = getVolume() + STEP_SIZE;
    if (newVolume < 255)
        setVolume(newVolume);
}

void decrementVolume() {
    int newVolume = getVolume() - STEP_SIZE;
    if (newVolume > 0)
        setVolume(newVolume);
}

void mute() {
    if (getVolume() != 0) {
        volumeBeforeMute = getVolume();
        setVolume(0);
    } else {
        setVolume(volumeBeforeMute);
    }
}

void setVolume(int vol) {
    volume = vol;
    VOL_CS = 0;
    putcSPI(volume); //Right channel
    putcSPI(volume); //Left channel
    VOL_CS = 1;
}

int getVolumeInDB() {
    Fraction fraction;
    Division division;
    fraction.numerator = getVolume() - 192;
    fraction.denuminator = 2;
    division = divide(fraction);
    return division.result;
}

int getVolumeInBars() {
    Fraction fraction;
    Division division;
    fraction.numerator = getVolume();
    fraction.denuminator = 11;
    division = divide(fraction);
    return division.result;
}

Division divide(Fraction fraction) {
    Division division;
    division.result = 0;
    division.remainder = fraction.numerator;

    if (fraction.numerator > 0) {
        while ((division.remainder - fraction.denuminator) >= 0) {
            division.remainder -= fraction.denuminator;
            ++division.result;
        }
    } else {
        while ((division.remainder + fraction.denuminator) <= 0) {
            division.remainder += fraction.denuminator;
            --division.result;
        }
    }

    return division;
}

