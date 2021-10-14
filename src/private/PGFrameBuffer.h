#ifndef PGFRAMEBUFFER_H
#define PGFRAMEBUFFER_H

#include "PixelGridTypes.h"
#include <Adafruit_ZeroDMA.h>
#include <SPI.h>
#include <wiring_private.h>

// neopixel string is connected to this pin
const int NEOPIXEL_PIN = 12;

// number of pixels in the frame buffer
const int NUMPIXELS = 13*13;

// Each pixel in the backbuffer has 4 bytes but we only use 3.  ARGB -> skip every 'A'
// Then each bit is expanded to 3 bits to for transmission to neopixels.  
// And finally there are 90 bytes of padding at the end of the buffer.
const int BYTES_PER_PIXEL = 3;
const int BITS_PER_BIT = 3;
const int FB_SIZE = (NUMPIXELS * BYTES_PER_PIXEL * BITS_PER_BIT * 8 + 7)/8 + 90;


class PGFrameBuffer
{
public:

	void begin();
		
	void setBrightness(uint8_t val);
	void setPixelColor(int addr, pgcolor color);
	pgcolor * getBackBuffer();
	void show();
	
protected:

	uint32_t expand_byte(uint8_t byte);

	int m_Brightness = 256;

	// Backbuffer, we draw into this buffer
	pgcolor m_BackBuffer[NUMPIXELS];

	// Frontbuffer, backbuffer is expanded into this buffer before DMA ships it out the SPI bus.
	uint8_t m_FrontBuffer[FB_SIZE];

	Adafruit_ZeroDMA m_Dma; 
	SPIClass *m_Spi;        

};


#endif //PGFRAMEBUFFER_H