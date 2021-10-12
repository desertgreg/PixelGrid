#include "PGGraphics.h"
#include <stdint.h>
#include <Arduino.h>
#include <SamD.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>

#define PIXEL_PIN   12 
#define PIXEL_COUNT 13*13 

Adafruit_NeoPixel_ZeroDMA g_FrameBuffer(PIXEL_COUNT, PIXEL_PIN, NEO_GRB);


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
		for (int i=dst_x0; i<dst_x1; ++i)
		{
			if (bmp.m_Pixels[src_y] & mask) 
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
