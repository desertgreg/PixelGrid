#ifndef PGRENDER_H
#define PGRENDER_H


#include <stdint.h>

#define PGCOLOR(r,g,b) ((r<<16) | (g<<8) | (b))
typedef uint32_t pgcolor;


class PGGraphics
{
public:

	static void Setup();
	static void Update();

	static void Clear();
	static void Fill(pgcolor color);
	static void SetPixel(uint8_t x,uint8_t y,pgcolor color);
};

#endif //PGBUTTONS_H
