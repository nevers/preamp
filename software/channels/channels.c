#include "channels.h"
#include "../volume/volume.h"
#include <p18cxxx.h>
#include <delays.h>

void clearAllChannels(void);
void clearMasterChannel(void);

int channel = -1;

void initAudioChannels() {
    CHANNEL1_PIN = 0;
    CHANNEL2_PIN = 0;
    CHANNEL3_PIN = 0;
    CHANNEL4_PIN = 0;
    CHANNEL5_PIN = 0;
    CHANNEL6_PIN = 0;
    MASTER_PIN = 0;
    clearAllChannels();
    clearMasterChannel();
}

void incrementChannel() {
    setChannel(channel + 1);
}

void decrementChannel() {
    setChannel(channel - 1);
}

int getChannel() {
    return channel;
}

void setChannel(int chan) {
    if (chan < 1)
        chan = 6;
    else if (chan > 6)
        chan = 1;

    if(chan == channel)
        return;

    channel = chan;

    if (getVolume() != 0)
        mute();
    
    clearAllChannels();
    Delay10KTCYx(20);

    switch (channel) {
        case 1:
            CHANNEL1 = 1;
            break;
        case 2:
            CHANNEL2 = 1;
            break;
        case 3:
            CHANNEL3 = 1;
            break;
        case 4:
            CHANNEL4 = 1;
            break;
        case 5:
            CHANNEL5 = 1;
            break;
        case 6:
            CHANNEL6 = 1;
            break;
    }
    mute();
}

void clearAllChannels() {
    CHANNEL1 = 0;
    CHANNEL2 = 0;
    CHANNEL3 = 0;
    CHANNEL4 = 0;
    CHANNEL5 = 0;
    CHANNEL6 = 0;
}

void clearMasterChannel() {
    MASTER = 0;
}

void enableMasterChannel() {
    MASTER = 1;
}

void toggleMasterChannel() {
    if(MASTER)
        clearMasterChannel();
    else
        enableMasterChannel();
}

