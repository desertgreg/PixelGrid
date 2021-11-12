
#include "PixelGrid.h"
#include "NanoBots8K.h"

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
  PixelGrid.setup();
  PixelGrid.playSound(Nanobots8KSnd,1,1);
}

void loop() {
  // put your main code here, to run repeatedly:
  PixelGrid.update();
}
