#include "PGGraphics.h"
#include "private/PGFrameBuffer.h"
#include <stdint.h>
#include <Arduino.h>
#include <SamD.h>

#define USE_ADAFRUIT 0
#define PIXEL_PIN   12 
#define PIXEL_COUNT 13*13 

#if USE_ADAFRUIT

#include <Adafruit_NeoPixel_ZeroDMA.h>
Adafruit_NeoPixel_ZeroDMA g_FrameBuffer(PIXEL_COUNT, PIXEL_PIN, NEO_GRB);

#else
PGFrameBuffer g_FrameBuffer;

#endif

void PGGraphics::setup()
{
	g_FrameBuffer.begin();
}

void PGGraphics::update()
{
	g_FrameBuffer.show();
}

void PGGraphics::clear()
{
	for (int i=0; i<PIXEL_COUNT; ++i)
	{
		g_FrameBuffer.setPixelColor(i,0);
	}
}

void PGGraphics::fill(pgcolor color)
{
	for (int i=0; i<PIXEL_COUNT; ++i)
	{
		g_FrameBuffer.setPixelColor(i,color);
	}
}

void PGGraphics::setPixel(int x,int y,pgcolor color)
{
	if ((x < 0) || (x >= 13)) return;
	if ((y < 0) || (y >= 13)) return;
	g_FrameBuffer.setPixelColor(x + y*13,color);
}

void PGGraphics::drawRow(int x0,int x1,int y, pgcolor color)
{
	if ((y < 0) || (y>=13)) return;
	if (x0 > x1) { int tmp = x0; x0 = x1; x1 = tmp; }
	if (x0 < 0) { x0 = 0; }
	if (x0 >= 13) return;
	if (x1 < 0) return;
	if (x1 >= 13) x1 = 12;
	
	pgcolor * fb = g_FrameBuffer.getBackBuffer();
	pgcolor * dest = fb + x0 + y*13;
	for (int i=0; i<x1-x0+1; ++i)
	{
		*dest++ = color;
	}
}

void PGGraphics::drawColumn(int x,int y0, int y1, pgcolor color)
{
	if ((x < 0) || (x>=13)) return;
	if (y0 > y1) { int tmp = y0; y0 = y1; y1 = tmp; }
	if (y0 >= 13) return;
	if (y1 < 0) return;
	if (y0 < 0) { y0 = 0; }
	if (y1 >= 13) { y1 = 12; }
	
	pgcolor * dest = g_FrameBuffer.getBackBuffer() + x + y0*13;
	for (int i=0; i<y1-y0+1; ++i)
	{
		*dest = color;
		dest+=13;
	}
}

void PGGraphics::drawBox(int x0,int y0, int x1,int y1, pgcolor color)
{
	drawRow(x0,x1,y0,color);
	drawRow(x0,x1,y1,color);
	drawColumn(x0,y0,y1,color);
	drawColumn(x1,y0,y1,color);
}

void PGGraphics::drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color)
{
	int src_x0 = 0;
	int src_y0 = 0;
	
	int dst_x0 = x;
	int dst_y0 = y;
	int dst_x1 = x+8;
	int dst_y1 = y+8;

	if (x < 0)
	{
		src_x0 = -x;
		dst_x0 = 0;
	}
	if (y < 0)
	{
		src_y0 = -y;
		dst_y0 = 0;
	}
	if (x+8 > 13)
	{
		dst_x1 = 13;
	}
	if (y+8 > 13)
	{
		dst_y1 = 13;
	}

	int src_y = src_y0;
	for (int j=dst_y0; j<dst_y1; ++j)
	{
		int mask = 0x80 >> src_x0;
		int pixels = bmp.m_Pixels[src_y];
		for (int i=dst_x0; i<dst_x1; ++i)
		{
			if (pixels & mask) 
			{
				g_FrameBuffer.setPixelColor(i+j*13,color);
			}
			mask = mask >> 1;
		}
		src_y++;
	}
}

void PGGraphics::drawBitmapInvert(int x, int y,PGBitmap8 & bmp,pgcolor color)
{
	int src_x0 = 0;
	int src_y0 = 0;
	
	int dst_x0 = x;
	int dst_y0 = y;
	int dst_x1 = x+8;
	int dst_y1 = y+8;

	if (x < 0)
	{
		src_x0 = -x;
		dst_x0 = 0;
	}
	if (y < 0)
	{
		src_y0 = -y;
		dst_y0 = 0;
	}
	if (x+8 > 13)
	{
		dst_x1 = 13;
	}
	if (y+8 > 13)
	{
		dst_y1 = 13;
	}
	int src_y = src_y0;
	for (int j=dst_y0; j<dst_y1; ++j)
	{
		int mask = 0x80 >> src_x0;
		uint8_t pixels = ~bmp.m_Pixels[src_y];
		for (int i=dst_x0; i<dst_x1; ++i)
		{
			if (pixels & mask) 
			{
				g_FrameBuffer.setPixelColor(i+j*13,color);
			}
			mask = mask >> 1;
		}
		src_y++;
	}
}


void PGGraphics::drawImage(int x, int y,PGImage & img)
{
	int src_x0 = 0;
	int src_y0 = 0;
	
	int dst_x0 = x;
	int dst_y0 = y;
	int dst_x1 = x+img.m_Width;
	int dst_y1 = y+img.m_Height;

	if (x < 0)
	{
		src_x0 = -x;
		dst_x0 = 0;
	}
	if (y < 0)
	{
		src_y0 = -y;
		dst_y0 = 0;
	}
	if (dst_x1 > 13)
	{
		dst_x1 = 13;
	}
	if (dst_y1 > 13)
	{
		dst_y1 = 13;
	}

	int src_y = src_y0;
	int stride = 4*img.m_Width;
	for (int j=dst_y0; j<dst_y1; ++j)
	{
		const uint8_t * src_pixel = &(img.m_PixelData[src_x0*4 + src_y*stride]);
		for (int i=dst_x0; i<dst_x1; ++i)
		{
			const int DIM = 4;
			pgcolor c = PGCOLOR(src_pixel[0]>>DIM,src_pixel[1]>>DIM,src_pixel[2]>>DIM);
			if (c > 0)
			{
				g_FrameBuffer.setPixelColor(i+j*13,c);
			}
			src_pixel+=4;
		}
		src_y++;
	}
}
