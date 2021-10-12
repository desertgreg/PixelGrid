#ifndef PGRENDER_H
#define PGRENDER_H

#include "..\PixelGridTypes.h"
#include <stdint.h>
#include <string.h>


class PGGraphics
{
public:
	static void setup();
	static void update();

	static void clear();
	static void fill(pgcolor color);
	static void setPixel(int x,int y,pgcolor color);
	static void drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color);
	static void drawImage(int x, int y,PGImage & img);
};

#endif //PGBUTTONS_H
