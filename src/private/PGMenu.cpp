#include "PGMenu.h"
#include "PGButtons.h"
#include "PGSounds.h"
#include "PGGraphics.h"
#include "PGSystemSounds.h"
#include "PGTransition.h"
#include "PixelGridTypes.h"

#define ICON_SIZE 8
#define ICON_SPACING 11

 
PGBitmap8 m_DogIcon(0x00,0x21,0x13,0x1f,0xe3,0xe3,0x22,0x66);
PGBitmap8 m_FaceIcon(0x00,0x24,0x7e,0x5a,0x7e,0x66,0x18,0x00);
PGBitmap8 m_BoxIcon(0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff);

//
// A mode to test out the menu system
//
class TestAppClass : public PGApp 
{
	void start() {}
	void update() {}
	PGBitmap8 & getIcon() { return m_FaceIcon; }
};
TestAppClass m_TestApp;


int PGMenu::m_curState = ST_NORMAL;
int PGMenu::m_appCount = 0;
int PGMenu::m_selectedApp = 0;
int PGMenu::m_stateCounter = 0;
PGApp * PGMenu::m_apps[MAX_APPS];
PGScrollerClass PGMenu::m_menuScroller;
PGTransition m_transition;	


void PGMenu::setup()
{
	// add the default modes, 
	m_apps[0] = &m_TestApp;
	m_apps[1] = &m_TestApp;
	m_apps[2] = &m_TestApp;
	m_apps[3] = &m_TestApp;
	m_appCount = 4;
	m_menuScroller.setImmediate(0);
	m_menuScroller.setSpeed(256);
}

const pgcolor ICON_COLOR = PGCOLOR(8,8,14);

void PGMenu::draw_icons(bool blink)
{
	// scroller goes from 0 to num_apps * icon_spacing
	// so you can think of it as a 'camera position' moving along the
	// row of icons.  To draw the icons, negate the position and offset a little
	int x = 2 - m_menuScroller.get();
	
	for (int i=0;i<m_appCount; ++i)
	{
		if (blink && i == m_selectedApp)
		{
			PGGraphics::drawBitmapInvert(x,2,m_apps[i]->getIcon(),ICON_COLOR);
		}
		else
		{
			PGGraphics::drawBitmap(x,2,m_apps[i]->getIcon(),ICON_COLOR);
		}
		x = x + ICON_SPACING;
	}
}

void PGMenu::update_normal()
{
	// draw the icons and check for scroll,click
	draw_icons(false);
	
	int pos = m_menuScroller.get();
	
	if (PGButtons::wasPressed(PGButton::R))
	{
		// if already at right end, buzz, otherwise trigger scroll
		if (pos >= (m_appCount - 1) * ICON_SPACING)
		{
			PGSounds::play(SwooshSnd);
		}
		else
		{
			m_menuScroller.setDelta(ICON_SPACING);
		}
	}
	else if (PGButtons::wasPressed(PGButton::L))
	{
		if (pos <= 0)
		{
			PGSounds::play(SwooshSnd);
		}
		else
		{
			m_menuScroller.setDelta(-ICON_SPACING);
		}
	}
	
	if (!m_menuScroller.isScrolling())
	{
		if (PGButtons::wasPressed(PGButton::OK) || PGButtons::wasPressed(PGButton::A))
		{
			m_selectedApp = pos/ICON_SPACING;
			m_curState = ST_BLINK;
			m_stateCounter = 0;
		}
	}
}

void PGMenu::update_blink()
{
	bool blink = (m_stateCounter & 0x08);  // alternate blink on/off as state counter increments
	draw_icons(blink);
	m_stateCounter++;
	if (m_stateCounter > 64)
	{
		// when blink is done, do the zoomin effect
		m_curState = ST_ZOOMIN;
		m_transition.start(PGTransition::ZOOM_OPEN);
	}
}

void PGMenu::update_zoomin()
{
	// todo zoomin effect and then activate the selected mode
	m_transition.update();
	if (m_transition.isActive() == false)
	{
		m_apps[m_selectedApp]->start();
		m_curState = ST_APP;
	}
}

void PGMenu::update_zoomout()
{
	m_transition.update();
	if (m_transition.isActive() == false)
	{
		m_curState = ST_NORMAL;
	}
}

void PGMenu::update_app()
{
	m_apps[m_selectedApp]->update();
	
	// check for exiting the app
	if (PGButtons::wasPressed(PGButton::OK))
	{
		m_transition.start(PGTransition::ZOOM_CLOSED);
		m_curState = ST_ZOOMOUT;
	}
}

void PGMenu::update()
{
	m_menuScroller.update();
	
	if (m_curState != ST_APP)
	{
		PGGraphics::clear();
	}
	
	// if a mode is running, just update the mode
	//  if OK pressed, start transition zoom out
	// draw row of icons
	//  if an icon is blinking update blink
	// if scrolling, update scroll
	// if on an icon
	//  if user pushes 'OK' or 'A' then trigger blink, lock out scrolling
	//  if blink finihses trigger transition
	//  if transition finishes 'start' a mode and set mode active
	switch (m_curState)
	{
		case ST_NORMAL:	// just draw the row of icons 
			update_normal();
			break;
		case ST_BLINK:	// draw row with center blinking
			update_blink();
			break;
		case ST_ZOOMIN:
			update_zoomin();
			break;
		case ST_ZOOMOUT:
			update_zoomout();
			break;
		case ST_APP:
			update_app();
			break;
		
	}
}

bool PGMenu::isActive()
{
}

void PGMenu::addApp(PGApp & app)
{
	if (m_appCount < MAX_APPS-1)
	{
		m_apps[m_appCount++] = &app;
	}
}
