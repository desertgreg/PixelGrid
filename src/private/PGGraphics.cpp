#include <stdint.h>
#include <Arduino.h>
#include <SamD.h>
#include "PGGraphics.h"
#include "private/PGFrameBuffer.h"


#define USE_ADAFRUIT 0
#define PIXEL_PIN   12 
#define PIXEL_COUNT 13*13 

#if USE_ADAFRUIT

#include <Adafruit_NeoPixel_ZeroDMA.h>
Adafruit_NeoPixel_ZeroDMA g_FrameBuffer(PIXEL_COUNT, PIXEL_PIN, NEO_GRB);

#else
PGFrameBuffer g_FrameBuffer;

#endif

//
// drawing state variables
//
static PGBlendMode g_blendMode = OPAQUE;
static uint8_t g_drawBrightness = 0x10;
static PGRenderTarget g_FrameBufferTarget;

//
// Clipped bitmap drawing
// making these global variables so we don't have to pass them all 
// on the stack.  This code will never be multithreaded so one function will
// set up the variables and the next can consume them.
//
static int g_srcX0;
static int g_srcY0;
static int g_dstX0;
static int g_dstX1;
static int g_dstY0;
static int g_dstY1;

////////////////////////////////////////////////////////////////////////////////////
//
//  Blend Functions
//
////////////////////////////////////////////////////////////////////////////////////

//
// blend_opaque - dst = src
//
class blend_opaque
{
public:
	static inline void blend(uint8_t * dst,const uint8_t * src)
	{
		// framebuffer is 0xaarrggbb, first byte is b, then g,r,a
		*dst++ = (*src++ * g_drawBrightness)>>8;  //b
		*dst++ = (*src++ * g_drawBrightness)>>8;  //g
		*dst++ = (*src++ * g_drawBrightness)>>8;  //r
		*dst++ = *src++;
	}
};


//
// blend additive - dst = src + dst
//
class blend_additive
{
public:
	static inline void blend(uint8_t * dst,const uint8_t * src)
	{
		// framebuffer is 0xaarrggbb, first byte is b, then g,r,a
		uint16_t tmp;
		tmp = *dst + (*src++ * g_drawBrightness)>>8;  //b
		if (tmp > 256) tmp = 255; // is there a clever way to saturate/clampe here?
		*dst++ = tmp;

		tmp = *dst + (*src++ * g_drawBrightness)>>8;  //g
		if (tmp > 256) tmp = 255;
		*dst++ = tmp;

		tmp = *dst + (*src++ * g_drawBrightness)>>8;  //r
		if (tmp > 256) tmp = 255; 
		*dst++ = tmp;
		
		*dst++ += *src++;
	}
};


//
// blend_alpha - dst = (src * a) + (dst * (1-a))
//
class blend_alpha
{
public:
	static inline void blend(uint8_t * dst,const uint8_t * src)
	{
		// framebuffer is 0xaarrggbb, first byte is b, then g,r,a
		// first read src r,g,b and apply brightness compensation
		uint8_t r,g,b,a;
		b = (uint8_t)((uint16_t)(*src++ * g_drawBrightness)>>8);
		g = (uint8_t)((uint16_t)(*src++ * g_drawBrightness)>>8);
		r = (uint8_t)((uint16_t)(*src++ * g_drawBrightness)>>8);
		a = (*src++);
		uint8_t ainv = 255-a;
		
		// blend with existing data in the framebuffer
		uint32_t tmp;
		tmp = (b * a) + (*dst * ainv) >> 8;
		*dst++ = (uint8_t)tmp;

		tmp = (g * a) + (*dst * ainv) >> 8;
		*dst++ = (uint8_t)tmp;
		
		tmp = (r * a) + (*dst * ainv) >> 8;
		*dst++ = (uint8_t)tmp;

		*dst++ = a;
	}
};

inline void blend(uint8_t * dst, uint8_t * src)
{
	switch(g_blendMode)
	{
		case OPAQUE:	blend_opaque::blend(dst,src); break;
		case ALPHA:		blend_alpha::blend(dst,src); break;
		case ADDITIVE:	blend_additive::blend(dst,src); break;
	}
}

////////////////////////////////////////////////////////////////////////////////////
//
//  PGGraphics Implementation
//
////////////////////////////////////////////////////////////////////////////////////

void PGGraphics::setup()
{
	g_FrameBuffer.begin();
	
	// set up the internal default render target to point at the frame buffer.
	// if we ever support user render targets, we would move this code to a function
	// that we run whenever they clear out their custom render target
	g_FrameBufferTarget.m_Width = 13;
	g_FrameBufferTarget.m_Height = 13;
	g_FrameBufferTarget.m_PixelData = (uint8_t*)g_FrameBuffer.getBackBuffer();
}

void PGGraphics::update()
{
	g_FrameBuffer.show();
}

void PGGraphics::setBlendMode(PGBlendMode bm)
{
	g_blendMode = bm;
}

void PGGraphics::setDrawBrightness(uint8_t bright)
{
	g_drawBrightness = bright;
	
	// neopixels are locking up if I go too bright!
	if (g_drawBrightness > 128) g_drawBrightness = 128;
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
	
	uint8_t* src = (uint8_t*)&color;
	uint8_t* dst = g_FrameBufferTarget.getPixelAddr(x,y);
	blend(dst,src);
}

void PGGraphics::drawRow(int x0,int x1,int y, pgcolor color)
{
	// clip the coordinates to the screen
	if ((y < 0) || (y>=13)) return;
	if (x0 > x1) { int tmp = x0; x0 = x1; x1 = tmp; }
	if (x0 < 0) { x0 = 0; }
	if (x0 >= 13) return;
	if (x1 < 0) return;
	if (x1 >= 13) x1 = 12;
	
	// fill the row
	uint8_t * dst = g_FrameBufferTarget.getPixelAddr(x0,y);
	uint8_t * src = (uint8_t*)&color;
	
	for (int i=0; i<x1-x0+1; ++i)
	{
		blend(dst,src);
		dst+=4;
	}
}

void PGGraphics::drawColumn(int x,int y0, int y1, pgcolor color)
{
	// clip the coordinates to the screen
	if ((x < 0) || (x>=13)) return;
	if (y0 > y1) { int tmp = y0; y0 = y1; y1 = tmp; }
	if (y0 >= 13) return;
	if (y1 < 0) return;
	if (y0 < 0) { y0 = 0; }
	if (y1 >= 13) { y1 = 12; }
	
	// draw the row
	uint8_t * dest = g_FrameBufferTarget.getPixelAddr(x,y0);
	uint8_t * src = (uint8_t*)&color;
	int stride = g_FrameBufferTarget.getStride();
	for (int i=0; i<y1-y0+1; ++i)
	{
		blend(dest,src);
		dest+=stride;
	}
}

void PGGraphics::drawBox(int x0,int y0, int x1,int y1, pgcolor color)
{
	if (x0 > x1) { uint8_t tmp = x0; x0 = x1; x1 = tmp; }
	if (x1 - x0 > 2)
	{
		drawRow(x0+1,x1-1,y0,color);
		drawRow(x0+1,x1-1,y1,color);
	}
	drawColumn(x0,y0,y1,color);
	drawColumn(x1,y0,y1,color);
}

void PGGraphics::drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color)
{
	// clip the src and dest rectangles
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

	uint8_t * src = (uint8_t*)&color;

	// copy the portion of the bitmap that is onscreen
	int src_y = src_y0;
	for (int j=dst_y0; j<dst_y1; ++j)
	{
		int mask = 0x80 >> src_x0;
		int pixels = bmp.m_Pixels[src_y];

		for (int i=dst_x0; i<dst_x1; ++i)
		{
			if (pixels & mask) 
			{
				uint8_t * dst = g_FrameBufferTarget.getPixelAddr(i,j);
				blend(dst,src);
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
	uint8_t * src = (uint8_t*)&color;
	for (int j=dst_y0; j<dst_y1; ++j)
	{
		int mask = 0x80 >> src_x0;
		uint8_t pixels = ~bmp.m_Pixels[src_y];
		uint8_t * dst = g_FrameBufferTarget.getPixelAddr(dst_x0,j);
		
		for (int i=dst_x0; i<dst_x1; ++i)
		{
			if (pixels & mask) 
			{
				blend(dst,src);
				dst+=4;
			}
			mask = mask >> 1;
		}
		src_y++;
	}
}


//
// clipImage - this takes the x,y coordinate of where we're going to draw the image
// and figures out the sub-rectangle of the source and destination to draw.  
// The outputs are:
// Subrect of the image to copy to the screen: (g_srcX0,g_src_Y0) .. (g_srcX1,g_srcY1) 
// Upper left location to copy the sub-rect to: (g_dstX0,g_dstY0) .. (g_dstX1,g_dstY1)
// (all values guaranteed to be within bounds)
//
void PGGraphics::clipImage(int x, int y,PGImage & img)
{
	g_srcX0 = 0;
	g_srcY0 = 0;
	
	g_dstX0 = x;
	g_dstY0 = y;
	g_dstX1 = x+img.m_Width;
	g_dstY1 = y+img.m_Height;

	if (x < 0)
	{
		g_srcX0 = -x;
		g_dstX0 = 0;
	}
	if (y < 0)
	{
		g_srcY0 = -y;
		g_dstY0 = 0;
	}
	if (g_dstX1 > 13)	// here we could support arbitrary render targets if we ever need to
	{
		g_dstX1 = 13;
	}
	if (g_dstY1 > 13)
	{
		g_dstY1 = 13;
	}
}


//
// drawImageClipped - uses the coordinate ranges computed by 'clipImage' and executes
// the given blend function on each pixel
//
template<typename blend>
void drawImageClipped(PGRenderTarget & dst,PGImage & src)
{
	int src_y = g_srcY0;
	int src_stride = 4*src.m_Width;
	int dst_stride = 4*dst.m_Width;
	for (int j=g_dstY0; j<g_dstY1; ++j)
	{
		const uint8_t * src_pixel = &(src.m_PixelData[g_srcX0*4 + src_y*src_stride]);
		uint8_t * dst_pixel = &(dst.m_PixelData[g_dstX0*4 + j*dst_stride]);
		for (int i=g_dstX0; i<g_dstX1; ++i)
		{
			blend::blend(dst_pixel,src_pixel);
			src_pixel += 4;
			dst_pixel += 4;
		}
		src_y++;
	}
}

//
// drawImage - Draw an image to the screen, handles cases where part of the image is off the screen (clipped)
//
void PGGraphics::drawImage(int x, int y,PGImage & img)
{
	clipImage(x,y,img);

	switch(g_blendMode)
	{
		case OPAQUE:
			drawImageClipped<blend_opaque>(g_FrameBufferTarget,img);
			break;
		
		case ALPHA:
			drawImageClipped<blend_alpha>(g_FrameBufferTarget,img);
			break;
		
		case ADDITIVE:
			drawImageClipped<blend_additive>(g_FrameBufferTarget,img);
			break;
	}
}

//
// Original code before we had brightness, blend modes, etc
//
void PGGraphics::drawImage2(int x, int y,PGImage & img)
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
    int stride = img.m_Width*4;
    for (int j=dst_y0; j<dst_y1; ++j)
    {
        const uint8_t * src_pixel = &(img.m_PixelData[src_x0 * 4 + src_y*stride]);
        for (int i=dst_x0; i<dst_x1; ++i)
        {
            const int DIM = 4;
            pgcolor c = PGCOLOR(src_pixel[2]>>DIM,src_pixel[1]>>DIM,src_pixel[0]>>DIM);
            if (c > 0)
            {
                g_FrameBuffer.setPixelColor(i+j*13,c);
            }
            src_pixel+=4;
        }
        src_y++;
    }
}
