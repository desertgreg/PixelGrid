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

	static void play(PGSound & sound);

};




#endif //PGSOUND_H
