#include "PixelGrid.h"

void setup() {
  PixelGrid.setup();
}

PGBitmap8 bmp(0x30,0x30,0x00,0x00,0x55,0xAA,0x00,0x03);
int x = 2;
int y = 2;

void loop() {

  if (PixelGrid.wasPressed(PGButton::U)) { y = y-1; }
  if (PixelGrid.wasPressed(PGButton::D)) { y = y+1; }
  if (PixelGrid.wasPressed(PGButton::L)) { x = x-1; }
  if (PixelGrid.wasPressed(PGButton::R)) { x = x+1; }

  PixelGrid.clear();
  PixelGrid.drawBitmap(x,y,bmp,PGCOLOR(0,100,210));
  PixelGrid.update();
}
