
#include "PixelGrid.h"


// variables for this sketch
int x = 6;
int y = 6;


void setup() 
{
  SerialUSB.begin(115200);
  PixelGrid.setup();
}

int wrap(int val) 
{
  if (val<0) val=12;
  if (val>12) val=0;
  return val;
}

void hello_world()
{
  // Use the buttons to move x,y around the screen
  if (PixelGrid.wasPressed(PGButton::U))
  {
    y=y-1;
    y=wrap(y);
  }
  if (PixelGrid.wasPressed(PGButton::D))
  {
    y=y+1;
    y=wrap(y);
  }
  if (PixelGrid.wasPressed(PGButton::L))
  {
    x=x-1;
    x=wrap(x);
  }
  if (PixelGrid.wasPressed(PGButton::R))
  {
    x=x+1;
    x=wrap(x);
  }

  // Play a sound if A is pressed
  if (PixelGrid.wasPressed(PGButton::A))
  {
    PixelGrid.playSound(TakeDamageSnd);
  }
  
  // Draw a dot at x,y
  PixelGrid.setPixel(x,y,PGCOLOR(10,3,4));
}

void loop() 
{
  // Clear the screen, update your game logic, draw the new frame.
  hello_world();

  // When you're done with the logic for a frame, let the system update
  PixelGrid.update();
}
