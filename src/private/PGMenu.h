#ifndef PGMODEMENU_H
#define PGMODEMENU_H

#include "PixelGridTypes.h"

static const int MAX_APPS = 32;

//
// Built-in menu system, lets the user scroll left and right through the app icons
// When the user selects one by pressing OK or A the icon blinks and the app starts
// At any time if the user hits 'OK' that takes you back to the main menu
//
class PGMenu 
{
public:

	static void setup();
	static void update();
	static bool isActive();

	static void addApp(PGApp & mode);
	
protected:
	
	static void draw_icons(bool blink);
	static void update_normal();
	static void update_blink();
	static void update_zoomin();
	static void update_zoomout();
	static void update_app();
	
	enum 
	{
		ST_NORMAL = 0,		// scrolling throught the icons
		ST_BLINK,			// selected an icon, blinking it
		ST_ZOOMIN,			// zoomin effect, before starting a mode
		ST_ZOOMOUT,			// zoomout effect, after exiting a mode
		ST_APP,				// an app is running...
	};
	
	static int m_curState;
	static int m_appCount;
	static int m_selectedApp;
	static int m_stateCounter;
	static PGApp * m_apps[MAX_APPS];
	
	static PGScrollerClass m_menuScroller;
};


#endif //PGMODEMENU_H