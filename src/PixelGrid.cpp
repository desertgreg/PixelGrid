#include "PixelGrid.h"
#include "private/PGFrameBuffer.h"
#include "private/PGTransition.h"
#include "private/PGMenu.h"


uint32_t s_LastMillis = 0;

PixelGridController PixelGrid;
PGTransition m_Transitions;



void PixelGridController::setup()
{
	PGButtons::setup();
	PGSounds::setup();
	PGGraphics::setup();
	m_introDone = false;
}


void PixelGridController::introUpdate()
{
	static int first = 1;
	static int startup_x = 15<<1;
	
	// Startup animation
	if (first == 1)
	{
		//PGSounds::play(PixelgridStartupSnd);
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
		m_introDone = true;
	}
}


void PixelGridController::update()
{
	if (!m_introDone) 
	{
		introUpdate();
	}
	else if (m_appsEnabled)
	{
		PGMenu::update();
	}
	
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

void PixelGridController::enableApps(bool onoff)
{
	PGMenu::setup();
	m_appsEnabled = onoff;
}

void PixelGridController::addApp(PGApp & app)
{
	PGMenu::addApp(app);
}

