#ifndef PGRENDER_H
#define PGRENDER_H


#include <stdint.h>
#include <string.h>

#define PGCOLOR(r,g,b) ((r<<16) | (g<<8) | (b))
typedef uint32_t pgcolor;


class PGBitmap8  // 8x8 single color bitmap
{
public:
	PGBitmap() { memset(m_Pixels,0,sizeof(m_Pixels)); }
	PGBitmap(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e,uint8_t f,uint8_t g,uint8_t h)
	{
		m_Pixels[0] = a; m_Pixels[1] = b; m_Pixels[2] = c; m_Pixels[3] = d;
		m_Pixels[4] = e; m_Pixels[5] = f; m_Pixels[6] = g; m_Pixels[7] = h;
	}
	uint8_t m_Pixels[8];
};


class PGGraphics
{
public:

	static void Setup();
	static void Update();

	static void Clear();
	static void Fill(pgcolor color);
	static void SetPixel(int x,int y,pgcolor color);
	static void DrawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color);
};

#endif //PGBUTTONS_H
