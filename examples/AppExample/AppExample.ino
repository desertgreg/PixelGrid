
#include "PixelGrid.h"



int wrap(int val) 
{
  if (val<0) val=12;
  if (val>12) val=0;
  return val;
}


// This example uses Apps.  You create apps and register them with the
// system.  Then you can run your app by navigating to its icon through
// the main menu.


// Implement an 'App' class which overrides the start, getIcon, and update functions:
class MyPaintApp : public PGApp
{
public:
  void start() override
  {
    PixelGrid.clear();  
  }

  PGImage & getIcon() override
  {
    return IconDefaultImg;  // TODO: make your own 13x13 icon image!
  }
  void update() override
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
    PixelGrid.setPixel(x,y,PGCOLOR(180,80,40));
  }

public:
  int x = 6;
  int y = 6;

};

// Create an instance of your app classe
MyPaintApp m_paint;


void setup() 
{
  SerialUSB.begin(115200);
  PixelGrid.setup();
  
  // Enable apps and add your App to the system
  PixelGrid.enableApps(true);
  PixelGrid.addApp(m_paint);
}


void loop() 
{
  // when using the app system, you don't need to do anthing in loop
  PixelGrid.update();
}
