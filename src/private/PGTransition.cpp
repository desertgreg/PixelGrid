#include "PGTransition.h"
#include "PixelGrid.h"
#include "PGGraphics.h"

PGTransition::PGTransition()
{
	m_transColor = PGCOLOR(180,180,200);
	m_transColor2 = PGCOLOR(80,80,90);
}

void PGTransition::start(PGTransitionVisual type)
{
	m_activeTransition = type;
	m_sweepCoord = 0;
	m_sweepRate = 128;
	//m_allowInterrupt = 0;
}

pgcolor TransitionColor = PGCOLOR(10,10,10);

void PGTransition::update_sweep_left()
{
	int coord = m_sweepCoord>>8;
	int x = 12-coord;
	int x1 = x+1;
	PGGraphics::drawColumn(x,0,12,m_transColor);
	PGGraphics::drawColumn(x1,0,12,m_transColor2);
	
	m_sweepCoord+=m_sweepRate;
	if (coord > 13) 
	{
		m_activeTransition = NONE;
	}
}

void PGTransition::update_sweep_right()
{
	int coord = m_sweepCoord>>8;
	int x = coord;
	int x1 = x-1;
	PGGraphics::drawColumn(x,0,12,m_transColor);
	PGGraphics::drawColumn(x1,0,12,m_transColor2);
	
	m_sweepCoord+=m_sweepRate;
	if (coord > 13) 
	{
		m_activeTransition = NONE;
	}
}

void PGTransition::update_sweep_up()
{
	int coord = m_sweepCoord>>8;
	int y = 12-coord;
	int y1 = y+1;
	PGGraphics::drawRow(0,12,y,m_transColor);
	PGGraphics::drawRow(0,12,y1,m_transColor2);
	
	m_sweepCoord+=m_sweepRate;
	if (coord > 13) 
	{
		m_activeTransition = NONE;
	}
}

void PGTransition::update_sweep_down()
{
	int coord = m_sweepCoord>>8;
	int y = coord;
	int y1 = y-1;
	PGGraphics::drawRow(0,12,y,m_transColor);
	PGGraphics::drawRow(0,12,y1,m_transColor2);
	
	m_sweepCoord+=m_sweepRate;
	if (coord > 13) 
	{
		m_activeTransition = NONE;
	}
}

void PGTransition::update_zoom_open()
{
	// m_SweepCoord goes from 0..7 in fixed point
	int coord = m_sweepCoord>>8;
	uint8_t c0 = 6-coord;
	uint8_t c1 = 6+coord;

	// Draw a box that gets bigger as 'sweep coord' gets bigger
	PixelGrid.drawBox(c0,c0,c1,c1,m_transColor);

	m_sweepCoord+=m_sweepRate;
	if (coord > 7)
	{
		m_activeTransition = NONE; 
	}
}

void PGTransition::update_zoom_closed()
{
	// m_SweepCoord goes from 0..7 in .4bit fixed point
	int coord = m_sweepCoord>>8;
	uint8_t c0 = coord;
	uint8_t c1 = 12-coord;
	
	PixelGrid.drawBox(c0,c0,c1,c1,m_transColor);
	
	m_sweepCoord+=m_sweepRate;
	if (coord > 5)
	{
		m_activeTransition = NONE; 
	}
}


void PGTransition::update()
{
	if (m_activeTransition == NONE) return;
	
	PGGraphics::clear();
	switch (m_activeTransition)
	{
		case SWEEP_LEFT: update_sweep_left(); break;
		case SWEEP_RIGHT: update_sweep_right(); break;
		case SWEEP_UP: update_sweep_up(); break;
		case SWEEP_DOWN: update_sweep_down(); break;
		case ZOOM_OPEN: update_zoom_open(); break;
		case ZOOM_CLOSED: update_zoom_closed(); break;
	}
}

bool PGTransition::isActive()
{
	return m_activeTransition != NONE;
}
