
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
	pgcolor c = PGCOLOR(30,60,20);
	float x,y;
	float radians = 2.0f * 3.141592654f * (float)ticker / 100.0f;
	x = 6.5f + 4.4f * sin(radians);
	y = 6.5f - 4.4f * cos(radians);
	PixelGrid.setPixel(x,y,c);
	x = 6.5f + 5.4f * sin(radians);
	y = 6.5f - 5.4f * cos(radians);
	PixelGrid.setPixel(x,y,c);
	x = 6.5f + 6.4f * sin(radians);
	y = 6.5f - 6.4f * cos(radians);
	PixelGrid.setPixel(x,y,c);
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

	int seconds = m_ticker / 100;
	int minutes = seconds / 60;
	int hundredths = m_ticker - seconds*100;
	
	// can't time longer than 9:59 minutes
	if (minutes > 9) 
	{
		m_timing = false;
		minutes = 9;
	}
	
	seconds = seconds - minutes * 60;
	int sectens = seconds / 10;
	seconds = seconds - sectens*10;
	int secones = seconds;
	
	PixelGrid.setBlendMode(PGBlendMode::ALPHA);
	PixelGrid.drawDigit(0,4,minutes);
	PixelGrid.setPixel(4,5,PGCOLOR(255,255,255));
	PixelGrid.setPixel(4,7,PGCOLOR(255,255,255));
	PixelGrid.drawDigit(6,4,sectens);
	PixelGrid.drawDigit(10,4,secones);
	PixelGrid.resetRenderStates();
	
	// Draw the hundredths in binary (0..100 so 7 digits)
	int xpos = 9;
	int ypos = 10;
	pgcolor c_one = PGCOLOR(120,120,160);
	pgcolor c_zero = PGCOLOR(32,64,64);

	for (int i=0; i<7; ++i)
	{
		pgcolor c = c_zero;
		if (hundredths & 0x01) 
		{ 
			c = c_one;
		}
		PixelGrid.setPixel(xpos,ypos,c);
		PixelGrid.setPixel(xpos,ypos+1,c);
		hundredths = hundredths>>1;
		xpos-=1;
	}
}

PGImage & PGTimerApp::getIcon()
{
	return IconStopwatchImg;
}
