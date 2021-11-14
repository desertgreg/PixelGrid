#include "PixelGrid.h"

void setup() {
  SerialUSB.begin(115200);
  PixelGrid.setup();
}

int counter = 0;
int val = 0;


void draw_background()
{
  pgcolor c0 = PGCOLOR(255,0,33);
  pgcolor c1 = PGCOLOR(33,0,255);
  pgcolor c;
  for (int i=0;i<13;++i)
  {
    c = PixelGrid.interpolateColors(c0,c1,(255*i)/12);
    PixelGrid.drawBox(i,0,i,12,c);
  }
}

void loop() {
  PixelGrid.clear();
  
  // put your main code here, to run repeatedly:
  counter++;
  if (counter > 10) 
  {
    val++;
    if (val > 999) val = 0;
    counter = 0;
  }

  draw_background();

  // pingpong counter 0..255..0..255, etc
  static uint16_t counter = 0;
  counter++;
  if (counter > 2*255<<4) counter = 0;
  int t = counter>>4;
  if (t > 255) t = 255 - (counter - 255);
  
  float g = 80.0f + 80.0f*sin(t);
  pgcolor c0 = PGCOLORA(255,160,64,255);
  pgcolor c1 = PGCOLORA(180,48,64,255);
  pgcolor c = PixelGrid.interpolateColors(c0,c1,t);
  
  PixelGrid.setTint(PGCOLORA(0,(int)g,0,255));
  PixelGrid.setBlendMode(PGBlendMode::ALPHA);
  
  PixelGrid.drawNumber(1,4,val);

  PixelGrid.update();
}
