#include "PixelGrid.h"
#include "private/PGFrameBuffer.h"
#include "private/PGTransition.h"
#include "private/PGMenu.h"


uint32_t s_LastMillis = 0;

PixelGridController PixelGrid;
PGTransition m_Transitions;
PGHardwareType m_hardware;

void PixelGridController::setup(PGHardwareType hw, PixelGridOptions options)
{
	m_hardware = hw;
	m_options = options;
	pinMode(PIN_LED,OUTPUT);
	SerialUSB.begin(115200);

	PGButtons::setup();
	
	if (m_options.m_enableSound)
	{
		PGSounds::setup();
	}
	
	switch (m_hardware)
	{
		case HW_PIXELGRID_16X8:
			m_introActive = false;
			m_width=16; m_height=8; m_indicators=m_options.m_stripLeds; 
			break;
		case HW_PIXELGRID_COLOR:
		default:
			m_introActive = true;
			m_width=13; m_height=13;m_indicators=6;  //6 built-in indicators, no strip connector
			break;
	}

	m_introActive &= m_options.m_enableIntro;
	PGGraphics::setup(m_width,m_height,m_indicators);
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
		m_introActive = false;
	}
}


void PixelGridController::update()
{
	PGGraphics::resetRenderStates();
	m_frameCounter++;
	
	if (m_introActive) 
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

	PGGraphics::resetRenderStates();

	// Pause before next pass through loop
	if (m_options.m_pageFlipMs > 0)
	{
		uint32_t cur = millis();
		while (cur - s_LastMillis < m_options.m_pageFlipMs)
		{
			cur = millis();
		}; 
		s_LastMillis = cur;
	}
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

