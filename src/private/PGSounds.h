#ifndef PGSOUND_H
#define PGSOUND_H

#include "Arduino.h"
#include "..\PixelGridTypes.h"
#include <stdint.h>


class PGSounds
{
public:
	
	static void setup();
	static void update();

	static void setChannelVolume(uint8_t channel,uint8_t volume);
	static void setChannelPlaybackRate(uint8_t channel,uint8_t rate);
	static void play(PGSound & sound,uint8_t channel = 0,uint8_t playback_rate = 0x7F, uint8_t loop = 0);

};




#endif //PGSOUND_H
