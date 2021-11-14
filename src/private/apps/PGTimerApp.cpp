
#include "PGTimerApp.h"

PGTimerApp::PGTimerApp()
{
}

void PGTimerApp::start() 
{
	m_ticker = 0;
}

void PGTimerApp::drawClockSweep(int ticker)
{
	float x,y;
	float radians = 2.0f * 3.141592654f * (float)ticker / 100.0f;
	x = 6.5f + 4.4f * sin(radians);
	y = 6.5f - 4.4f * cos(radians);
	PixelGrid.setPixel(x,y,PGCOLOR(20,32,15));
	x = 6.5f + 5.4f * sin(radians);
	y = 6.5f - 5.4f * cos(radians);
	PixelGrid.setPixel(x,y,PGCOLOR(20,35,15));
	x = 6.5f + 6.4f * sin(radians);
	y = 6.5f - 6.4f * cos(radians);
	PixelGrid.setPixel(x,y,PGCOLOR(20,35,25));
}

void PGTimerApp::reset()
{
	m_ticker = 0;
}

void PGTimerApp::toggleTiming()
{
	m_timing = !m_timing;
}




void PGTimerApp::update()
{
	PixelGrid.clear();

	if (m_timing)
	{
		m_ticker++;
	}
	
	if (PixelGrid.wasPressed(PGButton::A)) toggleTiming();
	if (PixelGrid.wasPressed(PGButton::B)) reset();
	
	if (m_timing)
		drawClockSweep(m_ticker);
}

PGImage & PGTimerApp::getIcon()
{
	return IconStopwatchImg;
}
