#ifndef PGFRAMEBUFFER_H
#define PGFRAMEBUFFER_H

#include "PixelGridTypes.h"
#include <Adafruit_ZeroDMA.h>
#include <SPI.h>
#include <wiring_private.h>

// neopixel string is connected to this pin
const int NEOPIXEL_PIN = 12;


// Each pixel in the backbuffer has 4 bytes but we only use 3.  ARGB -> skip every 'A'
// Then each bit is expanded to 3 bits to for transmission to neopixels.  
// And finally there are 90 bytes of padding at the end of the buffer.
const int BYTES_PER_PIXEL = 3;
const int BITS_PER_BIT = 3;
#define FB_SIZE(numpixels) ((numpixels) * BYTES_PER_PIXEL * BITS_PER_BIT * 8 + 7)/8 + 90

const int MAX_WIDTH = 20;
const int MAX_HEIGHT = 11;
const int MAX_INDICATORS = 256; // allows for a strip leds attached to 16x8 display
const int MAX_BACKBUFFER = MAX_WIDTH*MAX_HEIGHT + MAX_INDICATORS;
const int MAX_FRONTBUFFER = FB_SIZE(MAX_BACKBUFFER);

class PGFrameBuffer
{
public:

	void begin(int w,int h,int indicators);
		
	void setBrightness(uint8_t val);
	void setPixelColor(int addr, pgcolor color);
	pgcolor * getBackBuffer();
	void show();
	
	int getWidth() { return m_Width; }
	int getHeight() { return m_Height; }
	int getIndicatorCount() { return m_Indicators; }
	int getIndicatorOffset() { return m_IndicatorOffset; }
	
protected:

	uint32_t expand_byte(uint8_t byte);

	int m_Width;
	int m_Height;
	int m_Indicators;
	int m_IndicatorOffset;
	int m_FBSize;
	int m_NumPixels;
	
	int m_Brightness = 256;

	// Backbuffer, we draw into this buffer
	pgcolor m_BackBuffer[MAX_BACKBUFFER];

	// Frontbuffer, backbuffer is expanded into this buffer before DMA ships it out the SPI bus.
	uint8_t m_FrontBuffer[MAX_FRONTBUFFER];

	Adafruit_ZeroDMA m_Dma;
	SPIClass *m_Spi;

};



inline void PGFrameBuffer::setPixelColor(int addr, pgcolor color)
{
	m_BackBuffer[addr] = color;
}

inline pgcolor * PGFrameBuffer::getBackBuffer()
{
	return &(m_BackBuffer[0]);
}



#endif //PGFRAMEBUFFER_H