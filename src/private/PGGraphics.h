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
	static void drawRow(int x0,int x1,int y, pgcolor color);
	static void drawColumn(int x,int y0, int y1, pgcolor color);
	static void drawBox(int x0,int y0, int x1,int y1, pgcolor color);
	static void drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color);
	static void drawBitmapInvert(int x, int y,PGBitmap8 & bmp,pgcolor color);
	static void drawImage(int x, int y,PGImage & img);
};

#endif //PGBUTTONS_H
