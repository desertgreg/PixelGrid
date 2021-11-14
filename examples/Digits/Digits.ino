#include "PixelGrid.h"

void setup() {
  SerialUSB.begin(115200);
  PixelGrid.setup();
}

int counter = 0;
int val = 0;

int clamp_digit(int val)
{
  if (val < 0) val = 0;
  if (val > 9) val = 9;
  return val;
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

  int tmp = val;
  int hundreds = clamp_digit(tmp / 100);
  tmp -= hundreds * 100;
  int tens = clamp_digit(tmp / 10);
  tmp -= tens * 10;
  int ones = clamp_digit(tmp);

  PixelGrid.drawDigit(0,0,hundreds);
  PixelGrid.drawDigit(4,0,tens);
  PixelGrid.drawDigit(8,0,ones);

  PixelGrid.update();
}
