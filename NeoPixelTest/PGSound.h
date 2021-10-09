#ifndef PGSOUND_H
#define PGSOUND_H

#include "Arduino.h"
#include <stdint.h>

class PGSound
{
public:
	
	static void Init();
	static void Play(const uint8_t * snd, uint16_t snd_size);
	static void Update();
	
};

#endif //PGSOUND_H
