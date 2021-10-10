#include "PGGraphics.h"
#include <stdint.h>
#include <Arduino.h>
#include <SamD.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>

#define PIXEL_PIN   12 
#define PIXEL_COUNT 13*13 

Adafruit_NeoPixel_ZeroDMA g_FrameBuffer(PIXEL_COUNT, PIXEL_PIN, NEO_GRB);


void PGGraphics::Setup()
{
	g_FrameBuffer.begin();
}

void PGGraphics::Update()
{
	g_FrameBuffer.show();
}

void PGGraphics::Clear()
{
	for (int i=0; i<PIXEL_COUNT; ++i)
	{
		g_FrameBuffer.setPixelColor(i,0);
	}
}

void PGGraphics::Fill(pgcolor color)
{
	for (int i=0; i<PIXEL_COUNT; ++i)
	{
		g_FrameBuffer.setPixelColor(i,color);
	}
}

void PGGraphics::SetPixel(int x,int y,pgcolor color)
{
	if ((x < 0) || (x >= 13)) return;
	if ((y < 0) || (y >= 13)) return;
	g_FrameBuffer.setPixelColor(x + y*13,color);
}

void PGGraphics::DrawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color)
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
