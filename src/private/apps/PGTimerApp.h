#ifndef PGTIMERAPP_H
#define PGTIMERAPP_H

#include "PixelGrid.h"
#include "PixelGridTypes.h"


class PGTimerApp : public PGApp
{
public:
	PGTimerApp();
	void start() override;
	void update() override;
	PGImage & getIcon() override;

protected:
	void drawClockSweep(int ticker);
	void reset();
	void toggleTiming();
	
	uint32_t m_ticker = 0;
	bool m_timing = false;
	
};


#endif //PGTIMERAPP_H