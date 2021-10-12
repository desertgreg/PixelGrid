#include "PixelGrid.h"

uint32_t s_LastMillis = 0;

PixelGridController PixelGrid;


void PixelGridController::setup()
{
	PGButtons::setup();
	PGSounds::setup();
	PGGraphics::setup();
}

void introUpdate()
{
	static int first = 1;
	static int startup_x = 15<<1;
	static int done = 0;
	
	if (done) return;
	
	// Startup animation
	if (first == 1)
	{
		PGSounds::play(PixelgridStartupSnd);
		first = 0;
	}
	if ((startup_x>>1) > -PixelGridImg.m_Width-5)
	{
		PGGraphics::clear();
		PGGraphics::drawImage((startup_x>>1),0,PixelGridImg);
		startup_x--;
	}
	else
	{
		done = 1;
	}
}


void PixelGridController::update()
{
	introUpdate();
	
	PGButtons::update();
	PGSounds::update();
	PGGraphics::update();

	// Pause before next pass through loop
	uint32_t cur = millis();
	while (cur - s_LastMillis < 10)
	{
		cur = millis();
	}; 
	s_LastMillis = cur;
}

