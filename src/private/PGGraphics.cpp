#include <stdint.h>
#include <Arduino.h>
#include <SamD.h>
#include "PGGraphics.h"
#include "private/PGFrameBuffer.h"
#include "private/PGSystemImages.h"

#define USE_ADAFRUIT 0
#define PIXEL_PIN   12 

#if USE_ADAFRUIT

#include <Adafruit_NeoPixel_ZeroDMA.h>
#define PIXEL_COUNT 13*13 
Adafruit_NeoPixel_ZeroDMA g_FrameBuffer(PIXEL_COUNT, PIXEL_PIN, NEO_GRB);

#else
PGFrameBuffer g_FrameBuffer;

#endif

#define DEFAULT_DRAW_BRIGHTNESS 0x10
//
// drawing state variables
//
static PGBlendMode g_blendMode = OPAQUE;
static uint32_t g_tintR = 256;
static uint32_t g_tintG = 256;
static uint32_t g_tintB = 256;
static uint32_t g_tintA = 256;
static bool g_tintRGB = false;
static bool g_tintAlpha = false;
static uint8_t g_drawBrightness = DEFAULT_DRAW_BRIGHTNESS;
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

class blend_opaque_tint
{
public:
	static inline void blend(uint8_t * dst,const uint8_t * src)
	{
		// framebuffer is 0xaarrggbb, first byte is b, then g,r,a
		*dst++ = (((*src++ * g_tintB)>>8) * g_drawBrightness)>>8;  //b
		*dst++ = (((*src++ * g_tintG)>>8) * g_drawBrightness)>>8;  //g
		*dst++ = (((*src++ * g_tintR)>>8) * g_drawBrightness)>>8;  //r
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
		if (tmp > 255) tmp = 255; // is there a clever way to saturate/clampe here?
		*dst++ = tmp;

		tmp = *dst + (*src++ * g_drawBrightness)>>8;  //g
		if (tmp > 255) tmp = 255;
		*dst++ = tmp;

		tmp = *dst + (*src++ * g_drawBrightness)>>8;  //r
		if (tmp > 255) tmp = 255; 
		*dst++ = tmp;
		
		*dst++ += *src++;
	}
};


class blend_additive_tint
{
public:
	static inline void blend(uint8_t * dst,const uint8_t * src)
	{
		// framebuffer is 0xaarrggbb, first byte is b, then g,r,a
		uint16_t tmp;
		tmp = *dst + (((*src++ * g_tintB)>>8) * g_drawBrightness)>>8;  //b
		if (tmp > 255) tmp = 255; // is there a clever way to saturate/clampe here?
		*dst++ = tmp;

		tmp = *dst + (((*src++ * g_tintG)>>8) * g_drawBrightness)>>8;  //g
		if (tmp > 255) tmp = 255;
		*dst++ = tmp;

		tmp = *dst + (((*src++ * g_tintR)>>8) * g_drawBrightness)>>8;  //r
		if (tmp > 255) tmp = 255; 
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
		uint32_t r,g,b,a;
		b = (*src++ * g_drawBrightness)>>8;  //b
		g = (*src++ * g_drawBrightness)>>8;  //g
		r = (*src++ * g_drawBrightness)>>8;  //r
		a = (*src++ * 0x102) >> 8;  // we need a to be 256 when we read 255 and 0 when we read 0.

		// blend with existing data in the framebuffer
		if (a > 0)
		{
			uint32_t ainv = 256-a;
			uint32_t tmp;
			tmp = ((b * a) + (*dst * ainv)) >> 8;
			*dst++ = (uint8_t)tmp;

			tmp = ((g * a) + (*dst * ainv)) >> 8;
			*dst++ = (uint8_t)tmp;

			tmp = ((r * a) + (*dst * ainv)) >> 8;
			*dst++ = (uint8_t)tmp;

			*dst++ = a;
		}
	}
};

//
// blend_alpha - dst = (src * a) + (dst * (1-a))
//
class blend_alpha_tint
{
public:
	static inline void blend(uint8_t * dst,const uint8_t * src)
	{
		// framebuffer is 0xaarrggbb, first byte is b, then g,r,a
		// first read src r,g,b and apply brightness compensation
		uint8_t r,g,b,a;
		b = (uint8_t)((uint16_t)(((*src++ * g_tintB)>>8) * g_drawBrightness)>>8);
		g = (uint8_t)((uint16_t)(((*src++ * g_tintG)>>8) * g_drawBrightness)>>8);
		r = (uint8_t)((uint16_t)(((*src++ * g_tintR)>>8) * g_drawBrightness)>>8);
		a = (*src++ * 0x102) >> 8;  // we need a to be 256 when we read 255 and 0 when we read 0.
		a = (a * g_tintA)>>8;
		
		if (a > 0)
		{
			uint32_t ainv = 256-a;
			
			// blend with existing data in the framebuffer
			uint32_t tmp;
			tmp = ((b * a) + (*dst * ainv)) >> 8;
			*dst++ = (uint8_t)tmp;

			tmp = ((g * a) + (*dst * ainv)) >> 8;
			*dst++ = (uint8_t)tmp;
			
			tmp = ((r * a) + (*dst * ainv)) >> 8;
			*dst++ = (uint8_t)tmp;
		
			*dst++ = a;
		}
	}
};



inline void blend(uint8_t * dst, uint8_t * src)
{
	switch(g_blendMode)
	{
		case OPAQUE:	
			if (g_tintRGB)
				blend_opaque_tint::blend(dst,src); 
			else
				blend_opaque::blend(dst,src); 
			break;
		case ALPHA:		
			if (g_tintRGB || g_tintA)
				blend_alpha_tint::blend(dst,src); 
			else	
				blend_alpha::blend(dst,src); 
			break;
		case ADDITIVE:	
			if (g_tintRGB)
				blend_additive_tint::blend(dst,src);
			else
				blend_additive::blend(dst,src); 
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////
//
//  PGGraphics Implementation
//
////////////////////////////////////////////////////////////////////////////////////

void PGGraphics::setup(int w,int h,int indicators)
{
	g_FrameBuffer.begin(w,h,indicators);
	
	// set up the internal default render target to point at the frame buffer.
	// if we ever support user render targets, we would move this code to a function
	// that we run whenever they clear out their custom render target
	g_FrameBufferTarget.m_Width = w;
	g_FrameBufferTarget.m_Height = h;
	g_FrameBufferTarget.m_PixelData = (uint8_t*)g_FrameBuffer.getBackBuffer();
}

void PGGraphics::update()
{
	g_FrameBuffer.show();
}

void PGGraphics::resetRenderStates()
{
	g_blendMode = PGBlendMode::OPAQUE;
	setTint(PGCOLORA(255,255,255,255));
	setDrawBrightness(DEFAULT_DRAW_BRIGHTNESS);
}

void PGGraphics::setBlendMode(PGBlendMode bm)
{
	g_blendMode = bm;
}

void PGGraphics::setTint(pgcolor tint)
{
	// This math remaps 0..255 to 0..256 for each tint channel.  We do this
	// so if you use a tint of 0xFFFFFFFF you would get back exactly the original color.
	// this is because (x * 256) >> 8 = x;  in .8 fixed point, 1.0 = 256 or 0x100
	g_tintR = (PGCOLOR_GETR(tint) * 0x102) >> 8;
	g_tintG = (PGCOLOR_GETG(tint) * 0x102) >> 8;
	g_tintB = (PGCOLOR_GETB(tint) * 0x102) >> 8;
	g_tintA = (PGCOLOR_GETA(tint) * 0x102) >> 8;
	g_tintRGB = ((g_tintR != 256) || (g_tintG != 256) || (g_tintB != 256));
	g_tintAlpha = (g_tintA != 256);
}

void PGGraphics::setDrawBrightness(uint8_t bright)
{
	g_drawBrightness = bright;
	
	// neopixels are locking up if I go too bright!
	if (g_drawBrightness > 128) g_drawBrightness = 128;
}

void PGGraphics::clear()
{
	for (int j=0; j<g_FrameBufferTarget.m_Height; ++j)
	{
		for (int i=0; i<g_FrameBufferTarget.m_Width; ++i)
		{
			uint8_t* dst = g_FrameBufferTarget.getPixelAddr(i,j);
			pgcolor* dstcolor = (pgcolor*)dst;
			*dstcolor = 0;
		}
	}
	
	for (int i=0; i<g_FrameBuffer.getIndicatorCount(); ++i)
	{
		setIndicator(i,0);
	}
}

void PGGraphics::fill(pgcolor color)
{
	for (int j=0; j<g_FrameBufferTarget.m_Height; ++j)
	{
		for (int i=0; i<g_FrameBufferTarget.m_Width; ++i)
		{
			uint8_t * dst = g_FrameBufferTarget.getPixelAddr(i,j);
			blend(dst,(uint8_t*)(&color));
		}
	}
}

void PGGraphics::setPixel(int x,int y,pgcolor color)
{
	if ((x < 0) || (x >= g_FrameBufferTarget.getWidth())) return;
	if ((y < 0) || (y >= g_FrameBufferTarget.getHeight())) return;
	
	uint8_t* src = (uint8_t*)&color;
	uint8_t* dst = g_FrameBufferTarget.getPixelAddr(x,y);
	blend(dst,src);
}

//
// Set the color of the 6 indicator pixels below the screen, note that this code goes directly
// to the framebuffer, it is not routed through the potential future 'render target' code.  
// 
void PGGraphics::setIndicator(int a,pgcolor color)
{
	if ((a >= 0) && (a < g_FrameBuffer.getIndicatorCount()))
	{
		uint8_t* src = (uint8_t*)&color;
		pgcolor * dstcolor = g_FrameBuffer.getBackBuffer() + g_FrameBuffer.getIndicatorOffset() + a;
		uint8_t* dst = (uint8_t*)dstcolor;
		blend_opaque::blend(dst,src);
	}
}

void PGGraphics::drawRow(int x0,int x1,int y, pgcolor color)
{
	// clip the coordinates to the screen
	if ((y < 0) || (y>=g_FrameBufferTarget.getHeight())) return;
	if (x0 > x1) { int tmp = x0; x0 = x1; x1 = tmp; }
	if (x0 < 0) { x0 = 0; }
	if (x0 >= g_FrameBufferTarget.getWidth()) return;
	if (x1 < 0) return;
	if (x1 >= g_FrameBufferTarget.getWidth()) { x1 = g_FrameBufferTarget.getWidth()-1; }
	
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
	if ((x < 0) || (x>=g_FrameBufferTarget.getWidth())) return;
	if (y0 > y1) { int tmp = y0; y0 = y1; y1 = tmp; }
	if (y0 >= g_FrameBufferTarget.getHeight()) return;
	if (y1 < 0) return;
	if (y0 < 0) { y0 = 0; }
	if (y1 >= g_FrameBufferTarget.getHeight()) { y1 = g_FrameBufferTarget.getHeight()-1; }
	
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
	if (x+8 > g_FrameBufferTarget.getWidth())
	{
		dst_x1 = g_FrameBufferTarget.getWidth();
	}
	if (y+8 > g_FrameBufferTarget.getHeight())
	{
		dst_y1 = g_FrameBufferTarget.getHeight();
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
	if (x+8 > g_FrameBufferTarget.getWidth())
	{
		dst_x1 = g_FrameBufferTarget.getWidth();
	}
	if (y+8 > g_FrameBufferTarget.getHeight())
	{
		dst_y1 = g_FrameBufferTarget.getHeight();
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
	if (g_dstX1 > g_FrameBufferTarget.getWidth())	// here we could support arbitrary render targets if we ever need to
	{
		g_dstX1 = g_FrameBufferTarget.getWidth();
	}
	if (g_dstY1 > g_FrameBufferTarget.getHeight())
	{
		g_dstY1 = g_FrameBufferTarget.getHeight();
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
			if (g_tintRGB)
				drawImageClipped<blend_opaque_tint>(g_FrameBufferTarget,img);
			else
				drawImageClipped<blend_opaque>(g_FrameBufferTarget,img);
			break;
		
		case ALPHA:
			if (g_tintRGB||g_tintAlpha)
				drawImageClipped<blend_alpha_tint>(g_FrameBufferTarget,img);
			else
				drawImageClipped<blend_alpha>(g_FrameBufferTarget,img);
			break;
		
		case ADDITIVE:
			if (g_tintRGB)
				drawImageClipped<blend_additive_tint>(g_FrameBufferTarget,img);
			else
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
    if (dst_x1 > g_FrameBufferTarget.getWidth())
    {
        dst_x1 = g_FrameBufferTarget.getWidth();
    }
    if (dst_y1 > g_FrameBufferTarget.getHeight())
    {
        dst_y1 = g_FrameBufferTarget.getHeight();
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
                g_FrameBuffer.setPixelColor(i+j*g_FrameBufferTarget.getWidth(),c);
            }
            src_pixel+=4;
        }
        src_y++;
    }
}

void PGGraphics::drawDigit(int x, int y, int digit)
{
	static PGImage * digits[10] = 
	{
	  &Digit0Img,
	  &Digit1Img,
	  &Digit2Img,
	  &Digit3Img,
	  &Digit4Img,
	  &Digit5Img,
	  &Digit6Img,
	  &Digit7Img,
	  &Digit8Img,
	  &Digit9Img
	};
	if (digit < 0) digit = 0;
	if (digit > 9) digit = 9;
	drawImage(x,y,*digits[digit]);
}

void PGGraphics::drawNumber(int x,int y, int number)
{
	if (number < 0) number = 0;
	if (number > 999) number = 999;
	
	int tmp = number;
	int hundreds = tmp / 100;
	tmp -= hundreds * 100;
	int tens = tmp / 10;
	tmp -= tens * 10;
	int ones = tmp;

	PixelGrid.drawDigit(x,y,hundreds);
	PixelGrid.drawDigit(x+4,y,tens);
	PixelGrid.drawDigit(x+8,y,ones);
}

