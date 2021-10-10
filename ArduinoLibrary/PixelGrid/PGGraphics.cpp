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

void PGGraphics::SetPixel(uint8_t x,uint8_t y,pgcolor color)
{
	g_FrameBuffer.setPixelColor(x + y*13,color);
}
