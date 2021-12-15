#include "PGMenu.h"
#include "PGButtons.h"
#include "PGSounds.h"
#include "PGGraphics.h"
#include "PGSystemSounds.h"
#include "PGTransition.h"
#include "PixelGridTypes.h"
#include "apps/PGSnakeApp.h"
#include "apps/PGTimerApp.h"


#define ICON_SIZE 13
#define ICON_SPACING 13

 
//
// A mode to test out the menu system
//
class TestAppClass : public PGApp 
{
	void start() {}
	void update() {}
	PGImage & getIcon() { return IconStopwatchImg; }
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
	// add the default apps, 
	PGTimerApp * timer = new PGTimerApp();
	addApp(*timer);
	PGSnakeApp * snake = new PGSnakeApp;
	addApp(*snake);
	
	m_menuScroller.setImmediate(0);
	m_menuScroller.setSpeed(256);
}

const pgcolor ICON_COLOR = PGCOLOR(100,100,200);

void PGMenu::draw_icons(bool blink)
{
	// scroller goes from 0 to num_apps * icon_spacing
	// so you can think of it as a 'camera position' moving along the
	// row of icons.  To draw the icons, negate the position and offset a little
	int x = -m_menuScroller.get();
	for (int i=0;i<m_appCount; ++i)
	{
		if (blink && i == m_selectedApp)
		{
			PGGraphics::setBlendMode(SILHOUETTE);
			PGGraphics::drawImage(x,0,m_apps[i]->getIcon());
		}
		else
		{
			PGGraphics::setBlendMode(OPAQUE);
			PGGraphics::drawImage(x,0,m_apps[i]->getIcon());
		}
		x = x + ICON_SPACING;
	}
	PGGraphics::setBlendMode(OPAQUE);
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
