#include "PixelGrid.h"

void setup() {
  PixelGrid.setup();
}

int x = 0;
int y = 0;

void loop() {

  // Use the buttons to move x,y around the screen
  if (PixelGrid.isDown(PGButton::U))
  {
    y=y+1;
  }
  if (PGButtons::isDown(PGButton::D))
  {
    y=y-1;
  }
  if (PGButtons::isDown(PGButton::L))
  {
    x=x+1;
  }
  if (PGButtons::isDown(PGButton::R))
  {
    x=x-1;
  }

  PixelGrid.clear();
  PixelGrid.drawPixelmap(x>>3,y>>3,CharacterTestImg);
  
  PixelGrid.update();
}
