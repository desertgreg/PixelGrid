#ifndef PIXELGRIDTYPES_H
#define PIXELGRIDTYPES_H

#include <stdint.h>
#include <string.h>

//
// Pixelgrid buttons enumeration
//
enum PGButton : uint8_t
{
	U = 0,
	R,
	D,
	L,
	OK,
	A,
	B,

	BCOUNT
};

//
// Sound effect definition
//
class PGSound
{
public:
	PGSound(const int8_t * data,uint32_t size) : m_Size(size), m_Data(data) {}
	
	uint32_t m_Size;
	const int8_t * m_Data;
};


// In the PixelGridlibrary we will do everyting in ARGB byte order and at the frame-buffer level we'll reorder
// the bytes as needed by neopixels.  This means in memory a 32bit pixel looks like this:
//   0xaarrggbb
// And if you look at that pixel through a uint8_t pointer it looks like this:
//   b = pixel[0]
//   g = pixel[1]
//   r = pixel[2]
//   a = pixel[3]
// (This is little endian byte ordering)
#define PG_ASHIFT 24
#define PG_RSHIFT 16
#define PG_GSHIFT 8
#define PG_BSHIFT 0

#define PGCOLOR(r,g,b) ((r<<PG_RSHIFT) | (g<<PG_GSHIFT) | (b<<PG_BSHIFT))
#define PGCOLORA(r,g,b,a) ((r<<PG_RSHIFT) | (g<<PG_GSHIFT) | (b<<PG_BSHIFT))

typedef uint32_t pgcolor;

//
// Blend modes
//
enum PGBlendMode : uint8_t 
{ 
	OPAQUE = 0, 
	ALPHA, 
	ADDITIVE,
	SILHOUETTE
};


//
// 8x8 bit mask (1 bit per pixel)
//
class PGBitmap8  // 8x8 single color bitmap
{
public:
	PGBitmap8() { memset(m_Pixels,0,sizeof(m_Pixels)); }
	PGBitmap8(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e,uint8_t f,uint8_t g,uint8_t h)
	{
		m_Pixels[0] = a; m_Pixels[1] = b; m_Pixels[2] = c; m_Pixels[3] = d;
		m_Pixels[4] = e; m_Pixels[5] = f; m_Pixels[6] = g; m_Pixels[7] = h;
	}
	uint8_t m_Pixels[8];
};

// 
// RGBA Image
//
class PGImage
{
public:
	PGImage(const uint8_t * data, uint16_t w,uint16_t h) : m_Width(w),m_Height(h),m_PixelData(data) {}
	uint16_t m_Width;
	uint16_t m_Height;
	const uint8_t * m_PixelData;
};

//
// RGBA render target - image that can be drawn into - almost the same as PGImage but the pixel buffer is read-write
//
class PGRenderTarget
{
public:
	PGRenderTarget() {}
	PGRenderTarget(uint8_t * buffer, uint16_t w, uint16_t h) : m_Width(w),m_Height(h),m_PixelData(buffer) {}
	int getStride() { return m_Width * 4; }
	uint8_t * getPixelAddr(int x,int y) { return &m_PixelData[x*4 + y*m_Width*4]; }
	
	uint16_t m_Width = 0;
	uint16_t m_Height = 0;
	uint8_t * m_PixelData = nullptr;
};

//
// PGApp objects are like little applications.  Your game or application will be launched when the user selects
// it out of the main menu.  You just supply an icon and a 'start' and 'update' function.
//
class PGApp
{
public:
	virtual void start() = 0;
	virtual void update() = 0;
	virtual PGImage & getIcon() = 0;  // return a 13x13 icon
};



//
// This class will 'scroll' a value toward a target value internally using 8bit fixed point math.
// A velocity of 256 will move the value 1 unit per update (and updates are normally 100fps)
//
class PGScrollerClass
{
public:
	int16_t get() { return m_pos>>8; }
	void setDest(int16_t val) { m_destPos = val<<8; }
	void setDelta(int16_t val) { m_destPos = m_destPos + (val<<8); }
	void setImmediate(int16_t val) { m_pos = val<<8; m_destPos = val<<8; }
	void setSpeed(int16_t spd) { m_speed = spd; }
	bool isDone() { return m_pos == m_destPos; }
	bool isScrolling() { return m_pos != m_destPos; }
	
	void debugPrint();

	void update() 
	{ 
		if (m_pos != m_destPos)
		{
			if (m_pos < m_destPos + m_speed) { m_pos += m_speed; }
			else if (m_pos > m_destPos - m_speed) { m_pos -= m_speed; }
			else { m_pos = m_destPos; }
		}
	}
	
protected:
	int m_pos = 0;
	int m_destPos = 0;
	uint16_t m_speed = 256;
};



#endif //PIXELGRIDTYPES_H
