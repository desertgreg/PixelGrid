#include "PixelGrid.h"


uint32_t s_LastMillis = 0;

void PixelGrid::Setup()
{
	PGButtons::Setup();
	PGSounds::Setup();
	PGGraphics::Setup();
}

void PixelGrid::Update()
{
	PGButtons::Update();
	PGSounds::Update();
	PGGraphics::Update();

	// Pause before next pass through loop
	uint32_t cur = millis();
	while (cur - s_LastMillis < 10)
	{
		cur = millis();
	}; 
	s_LastMillis = cur;
}

