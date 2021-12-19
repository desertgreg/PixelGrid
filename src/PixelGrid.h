
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
	
	uint32_t getFrame() { return m_frameCounter; }
	
	// Buttons
	uint8_t isDown(PGButton b);
	uint8_t wasPressed(PGButton b);
	uint8_t wasReleased(PGButton b);
	uint8_t anyPressed();
	
	// Sound
	void setChannelVolume(uint8_t channel,uint8_t volume);
	void playSound(PGSound & sound,uint8_t channel = 0,uint8_t loop = 0);

	// Graphics
	void resetRenderStates();
	void setBlendMode(PGBlendMode bm);
	void setTint(pgcolor tint);
	void setBrightness(uint8_t bright);
	
	void clear();
	void fill(pgcolor color);
	void setPixel(int x,int y,pgcolor color);
	void setIndicator(int i,pgcolor color);
	void drawRow(int x0,int x1,int y,pgcolor color);
	void drawColumn(int x,int y0,int y1,pgcolor color);
	void drawBox(int x0,int y0,int x1, int y1,pgcolor color);
	void drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color);
	void drawImage(int x, int y,PGImage & pmp);
	void drawDigit(int x, int y, int digit);
	void drawNumber(int x, int y, int number);
	
	// Utility
	pgcolor interpolateColors(pgcolor a, pgcolor b, uint8_t blend);
	
	// Debugging
	void print(int val);
	void print(char * string);
	void print(pgcolor c);
	void print(char * label, int val);
	
	// Apps (optional, enables a menu and multiple applications, add your game using 'addApp')
	void enableApps(bool onoff = true);
	void addApp(PGApp & app);

private:
	void introUpdate();
	
	bool m_appsEnabled = false;
	bool m_introDone = false;
	uint32_t m_frameCounter = 0;
};


// Global instance of the PixelGridController
extern PixelGridController PixelGrid;


// Inline functions to call into the pixelgrid subsystems.
inline uint8_t PixelGridController::isDown(PGButton b) { return PGButtons::isDown(b); }
inline uint8_t PixelGridController::wasPressed(PGButton b) { return PGButtons::wasPressed(b); }
inline uint8_t PixelGridController::wasReleased(PGButton b) { return PGButtons::wasReleased(b); }
inline uint8_t PixelGridController::anyPressed() { return PGButtons::wasPressed(PGButton::U) || PGButtons::wasPressed(PGButton::D) || PGButtons::wasPressed(PGButton::L)|| PGButtons::wasPressed(PGButton::R)|| PGButtons::wasPressed(PGButton::A)|| PGButtons::wasPressed(PGButton::B); }

inline void PixelGridController::setChannelVolume(uint8_t channel,uint8_t volume) { PGSounds::setChannelVolume(channel,volume); }
inline void PixelGridController::playSound(PGSound & sound,uint8_t channel,uint8_t loop) { PGSounds::play(sound,channel,loop); }

inline void PixelGridController::resetRenderStates() { PGGraphics::resetRenderStates(); }
inline void PixelGridController::setBlendMode(PGBlendMode bm) { PGGraphics::setBlendMode(bm); }
inline void PixelGridController::setTint(pgcolor tint) { PGGraphics::setTint(tint); }
inline void PixelGridController::setBrightness(uint8_t bright) { PGGraphics::setDrawBrightness(bright); }
inline void PixelGridController::clear() { PGGraphics::clear(); }
inline void PixelGridController::fill(pgcolor color) { PGGraphics::fill(color); }
inline void PixelGridController::setPixel(int x,int y,pgcolor color) { PGGraphics::setPixel(x,y,color); }
inline void PixelGridController::setIndicator(int i,pgcolor color) { PGGraphics::setIndicator(i,color); }
inline void PixelGridController::drawRow(int x0,int x1,int y,pgcolor color) { PGGraphics::drawRow(x0,x1,y,color); }
inline void PixelGridController::drawColumn(int x,int y0,int y1,pgcolor color) { PGGraphics::drawRow(x,y0,y1,color); }

inline void PixelGridController::drawBox(int x0,int y0,int x1, int y1,pgcolor color) { PGGraphics::drawBox(x0,y0,x1,y1,color); }
inline void PixelGridController::drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color) { PGGraphics::drawBitmap(x,y,bmp,color); }
inline void PixelGridController::drawImage(int x, int y,PGImage & pmp) { PGGraphics::drawImage(x,y,pmp); }
inline void PixelGridController::drawDigit(int x, int y, int digit) { PGGraphics::drawDigit(x,y,digit); }
inline void PixelGridController::drawNumber(int x,int y, int number) { PGGraphics::drawNumber(x,y,number); }

inline pgcolor PixelGridController::interpolateColors(pgcolor c0, pgcolor c1, uint8_t blend)
{
	uint8_t invblend = 255-blend;
	uint16_t r = (((uint16_t)PGCOLOR_GETR(c0) * invblend) + ((uint16_t)PGCOLOR_GETR(c1) * blend))>>8;
	uint16_t g = (((uint16_t)PGCOLOR_GETG(c0) * invblend) + ((uint16_t)PGCOLOR_GETG(c1) * blend))>>8;
	uint16_t b = (((uint16_t)PGCOLOR_GETB(c0) * invblend) + ((uint16_t)PGCOLOR_GETB(c1) * blend))>>8;
	uint16_t a = (((uint16_t)PGCOLOR_GETA(c0) * invblend) + ((uint16_t)PGCOLOR_GETA(c1) * blend))>>8;
	return PGCOLORA(r,g,b,a);
}

inline void PixelGridController::print(int val)
{
	SerialUSB.print(val);
	SerialUSB.print("\r\n");
}
inline void PixelGridController::print(char * string)
{
	SerialUSB.print(string);
	SerialUSB.print("\r\n");
}

inline void PixelGridController::print(pgcolor c)
{
	SerialUSB.print("r:");
	SerialUSB.print(PGCOLOR_GETR(c));
	SerialUSB.print(" g:");
	SerialUSB.print(PGCOLOR_GETG(c));
	SerialUSB.print(" b:");
	SerialUSB.print(PGCOLOR_GETB(c));
	SerialUSB.print(" a:");
	SerialUSB.print(PGCOLOR_GETA(c));
	SerialUSB.print("\r\n");
}

inline void PixelGridController::print(char * label, int val)
{
	SerialUSB.print(label);
	SerialUSB.print(" ");
	SerialUSB.print(val);
	SerialUSB.print("\r\n");
}

#endif //PIXELGRID_H