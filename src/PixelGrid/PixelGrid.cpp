#include "PixelGrid.h"
#include "PGSystemImages.h"

uint32_t s_LastMillis = 0;

void PixelGrid::Setup()
{
	PGButtons::Setup();
	PGSounds::Setup();
	PGGraphics::Setup();
}

int first = 1;
int startup_x = 15<<1;

void PixelGrid::Update()
{
	// Startup animation
	if (first == 1)
	{
		PGSounds::Play(PixelgridStartupSnd);
		first = 0;
	}
	if ((startup_x>>1) > -PixelGridImg.m_Width-5)
	{
		PGGraphics::Clear();
		PGGraphics::DrawPixelmap((startup_x>>1),0,PixelGridImg);
		startup_x--;
	}

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

