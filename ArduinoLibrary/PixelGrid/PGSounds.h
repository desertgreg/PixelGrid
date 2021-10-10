#ifndef PGSOUND_H
#define PGSOUND_H

#include "Arduino.h"
#include <stdint.h>


class PGSound
{
public:
	PGSound(const uint8_t * data,uint16_t size) : m_Size(size), m_Data(data) {}
	
	uint16_t m_Size;
	const uint8_t * m_Data;
};


class PGSounds
{
public:
	
	static void Setup();
	static void Play(PGSound & sound);
	static void Update();
	
	// 
	// Built-in sounds
	//
	static PGSound Powerup;
	static PGSound Blip;

};




#endif //PGSOUND_H
