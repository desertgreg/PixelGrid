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
	PGSound(const uint8_t * data,uint16_t size) : m_Size(size), m_Data(data) {}
	
	uint16_t m_Size;
	const uint8_t * m_Data;
};


// AGRB byte order but little endian so B is in the MSB.
#define PGCOLOR(r,g,b) ((b<<24) | (r<<16) | (g<<8) )
typedef uint32_t pgcolor;


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
// PGApp objects are like little applications.  Your game or application will be launched when the user selects
// it out of the main menu.  You just supply an icon and a 'start' and 'update' function.
//
class PGApp
{
public:
	virtual void start() = 0;
	virtual void update() = 0;
	virtual PGBitmap8 & getIcon() = 0;
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
