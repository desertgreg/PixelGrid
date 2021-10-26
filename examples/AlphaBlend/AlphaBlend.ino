//
// This example demponstrates alpha blending and brightness.  4 circles are drawn using alpha-blending
// The neopixel display is very bright so under most circumstances the brightness needs to be set to around 16-20 (out of 255)
// 

#include "PixelGrid.h"
#include "GfxTest.h"

void setup() {
  PixelGrid.setup();
  SerialUSB.begin(114200);
  //while (!SerialUSB);
}



int x=0;
int y=0;
int bright = 0x10;

void loop() 
{
  if (PixelGrid.wasPressed(PGButton::U)) { y = y-1; }
  if (PixelGrid.wasPressed(PGButton::D)) { y = y+1; }
  if (PixelGrid.wasPressed(PGButton::L)) { x = x-1; }
  if (PixelGrid.wasPressed(PGButton::R)) { x = x+1; }
  
  if (PixelGrid.wasPressed(PGButton::A)) { bright += 4; PixelGrid.playSound(MenuMoveSnd);SerialUSB.print(bright);}
  if (PixelGrid.wasPressed(PGButton::B)) { bright -= 4; PixelGrid.playSound(SwooshSnd); SerialUSB.print(bright);}

  // update the brightness
  if (bright < 0) bright = 0;
  if (bright > 128) bright = 128;
  PixelGrid.setBrightness(bright);

  // draw some circles with blending
  static float t=0.0f;
  t+=0.05f;
  float cx = 6.0 + 10.0*sin(t);
  float cy = 4.0 + 11.0*sin(0.9*t);
  float cx2 = 1.5 + 4*cos(0.7*t + 0.5);
  float cy2 = 3 + 8*sin(0.5*t + 0.2);
  
  PixelGrid.clear();
  PixelGrid.setBlendMode(OPAQUE);

  PixelGrid.drawImage(x,y,RedCircleImg);

  PixelGrid.setBlendMode(ALPHA);
  PixelGrid.drawImage(cx,1,BlueCircleImg);
  PixelGrid.drawImage(2,cy,WiteCircleImg);
  PixelGrid.drawImage(cx2,cy2,GreenCircleImg);
  
  PixelGrid.update();

}
