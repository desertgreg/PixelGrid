
#include "PixelGrid.h"


void setup() 
{
  PixelGrid.setup();
}

void loop() 
{
  // clear the screen
  PixelGrid.clear();
  
  // Play a sound if A is pressed
  if (PixelGrid.wasPressed(PGButton::A))
  {
    PixelGrid.playSound(TakeDamageSnd);
  }
  
  // Draw a dot at x,y
  PixelGrid.setPixel(6,6,PGCOLOR(190,64,20));

  // When you're done with the logic for a frame, let the system update
  PixelGrid.update();
}
