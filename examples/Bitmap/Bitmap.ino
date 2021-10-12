#include "PixelGrid.h"

void setup() {
  PixelGrid::Setup();
}

PGBitmap8 bmp(0x30,0x30,0x00,0x00,0x55,0xAA,0x00,0x03);
int x = 6;
int y = 6;

void loop() {

  if (PGButtons::Was_Pressed(PGButtons::U)) { y = y-1; }
  if (PGButtons::Was_Pressed(PGButtons::D)) { y = y+1; }
  if (PGButtons::Was_Pressed(PGButtons::L)) { x = x-1; }
  if (PGButtons::Was_Pressed(PGButtons::R)) { x = x+1; }

  PGGraphics::Clear();
  PGGraphics::DrawBitmap(x,y,bmp,PGCOLOR(0,4,7));
  PixelGrid::Update();
}
