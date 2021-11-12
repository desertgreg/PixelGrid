

#include "PixelGrid.h"
#include "BitsAndBeats.h"

void setup() {
  // put your setup code here, to run once:
  PixelGrid.setup();
  PixelGrid.playSound(BitsAndBeats8kSnd,1,1);
}

void loop() {
  // put your main code here, to run repeatedly:
  PixelGrid.update();
}
