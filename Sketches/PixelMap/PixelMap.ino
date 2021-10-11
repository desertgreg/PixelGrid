#include "PixelGrid.h"

void setup() {
  PixelGrid::Setup();
}

int x = 0;
int y = 0;

void loop() {

  // Use the buttons to move x,y around the screen
  if (PGButtons::Is_Down(PGButtons::U))
  {
    y=y+1;
  }
  if (PGButtons::Is_Down(PGButtons::D))
  {
    y=y-1;
  }
  if (PGButtons::Is_Down(PGButtons::L))
  {
    x=x+1;
  }
  if (PGButtons::Is_Down(PGButtons::R))
  {
    x=x-1;
  }

  PGGraphics::Clear();  
  PGGraphics::DrawPixelmap(x>>3,y>>3,CharacterTestImg);
  
  PixelGrid::Update();
}
