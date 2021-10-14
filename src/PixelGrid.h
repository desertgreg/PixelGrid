
#ifndef PIXELGRID_H
#define PIXELGRID_H

#include "PixelGridTypes.h"
#include "private\PGButtons.h"
#include "private\PGSounds.h"
#include "private\PGGraphics.h"
#include "private\PGSystemSounds.h"
#include "private\PGSystemImages.h"


//
// PixelGrid API
// 
class PixelGridController
{
public:
	void setup();
	void update();

	// Buttons
	uint8_t isDown(PGButton b);
	uint8_t wasPressed(PGButton b);
	uint8_t wasReleased(PGButton b);
	
	// Sound
	void playSound(PGSound & sound);

	// Graphics
	void clear();
	void fill(pgcolor color);
	void setPixel(int x,int y,pgcolor color);
	void drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color);
	void drawImage(int x, int y,PGImage & pmp);

};

// Global instance of the PixelGridController
extern PixelGridController PixelGrid;



// Inline functions to call into the pixelgrid subsystems.
inline uint8_t PixelGridController::isDown(PGButton b) { return PGButtons::isDown(b); }
inline uint8_t PixelGridController::wasPressed(PGButton b) { return PGButtons::wasPressed(b); }
inline uint8_t PixelGridController::wasReleased(PGButton b) { return PGButtons::wasReleased(b); }

inline void PixelGridController::playSound(PGSound & sound) { PGSounds::play(sound); }

inline void PixelGridController::clear() { PGGraphics::clear(); }
inline void PixelGridController::fill(pgcolor color) { PGGraphics::fill(color); }
inline void PixelGridController::setPixel(int x,int y,pgcolor color) { PGGraphics::setPixel(x,y,color); }
inline void PixelGridController::drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color) { PGGraphics::drawBitmap(x,y,bmp,color); }
inline void PixelGridController::drawImage(int x, int y,PGImage & pmp) { PGGraphics::drawImage(x,y,pmp); }


#endif //PIXELGRID_H