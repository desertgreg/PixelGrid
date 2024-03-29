#ifndef PGRENDER_H
#define PGRENDER_H

#include "../PixelGridTypes.h"
#include <stdint.h>
#include <string.h>


class PGGraphics
{
public:
	static void setup(int w,int h,int indicators);
	static void update();

	// drawing state
	static void resetRenderStates();
	static void setBlendMode(PGBlendMode bm);
	static void setTint(pgcolor tint);
	static void setDrawBrightness(uint8_t bright);
	
	static void clear();
	static void fade(uint8_t amount);
	static void fill(pgcolor color);
	static void setPixel(int x,int y,pgcolor color);
	static void setIndicator(int a,pgcolor color);
	static void drawRow(int x0,int x1,int y, pgcolor color);
	static void drawColumn(int x,int y0, int y1, pgcolor color);
	static void drawBox(int x0,int y0, int x1,int y1, pgcolor color,bool fill);
	static void drawBitmap(int x, int y,PGBitmap8 & bmp,pgcolor color);
	static void drawBitmapInvert(int x, int y,PGBitmap8 & bmp,pgcolor color);
	static void drawImage(int x, int y,PGImage & img);
	static void drawImage2(int x, int y,PGImage & img);
	static void drawDigit(int x, int y, int digit);
	static void drawNumber(int x,int y, int number);
	
private:
	static void clipImage(int x, int y,PGImage & img);

};

#endif //PGBUTTONS_H
