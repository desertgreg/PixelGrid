#ifndef PGTRANSITION_H
#define PGRTANSITION_H

#include "PixelGridTypes.h"


//
// This class can animate some effects on the screen to indicate transitions
//
class PGTransition 
{
public:
	enum PGTransitionVisual
	{
		NONE = 0,
		SWEEP_LEFT,
		SWEEP_RIGHT,
		SWEEP_UP,
		SWEEP_DOWN,
		ZOOM_OPEN,
		ZOOM_CLOSED,
	};

	PGTransition();
	
	void start(PGTransitionVisual type);
	void update();
	bool isActive();

protected:

	PGTransitionVisual m_activeTransition;
	int m_sweepCoord;	// note, this has 8bits of fraction
	int m_sweepRate;	// how much to increment the sweep per frame (1/100s).  255 = 1 pixel per frame
	pgcolor m_transColor;
	pgcolor m_transColor2;
	
	void update_sweep_left(); 
	void update_sweep_right();
	void update_sweep_up();
	void update_sweep_down();
	void update_zoom_open();
	void update_zoom_closed();

};



#endif //PGTRANSITION_H