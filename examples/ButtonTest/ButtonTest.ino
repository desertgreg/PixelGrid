#include "PixelGrid.h"


// This Sketch just tests the button functions.
// The first 7 columns of pixels display the button state.
// The first 3 rows show the debounced button state: isDown, wasPressed, and wasReleased
// The last row shows the hardware state of the button with no debouncing (you need an oscilloscope to really see the 'bouncing' though)

void setup() {
  PixelGrid.setup();
  SerialUSB.begin(114200);
  while (!SerialUSB);
}

const pgcolor ONCOLOR = PGCOLOR(10,7,3);

void loop() {
  PixelGrid.clear();
  if (PixelGrid.isDown(PGButton::U)) { PixelGrid.setPixel(0,0,ONCOLOR); }
  if (PixelGrid.isDown(PGButton::R)) { PixelGrid.setPixel(1,0,ONCOLOR); }
  if (PixelGrid.isDown(PGButton::D)) { PixelGrid.setPixel(2,0,ONCOLOR); }
  if (PixelGrid.isDown(PGButton::L)) { PixelGrid.setPixel(3,0,ONCOLOR); }
  if (PixelGrid.isDown(PGButton::OK)) { PixelGrid.setPixel(4,0,ONCOLOR); }
  if (PixelGrid.isDown(PGButton::A)) { PixelGrid.setPixel(5,0,ONCOLOR); }
  if (PixelGrid.isDown(PGButton::B)) { PixelGrid.setPixel(6,0,ONCOLOR); }

  if (PixelGrid.wasPressed(PGButton::U)) { PixelGrid.setPixel(0,1,ONCOLOR); }
  if (PixelGrid.wasPressed(PGButton::R)) { PixelGrid.setPixel(1,1,ONCOLOR); }
  if (PixelGrid.wasPressed(PGButton::D)) { PixelGrid.setPixel(2,1,ONCOLOR); }
  if (PixelGrid.wasPressed(PGButton::L)) { PixelGrid.setPixel(3,1,ONCOLOR); }
  if (PixelGrid.wasPressed(PGButton::OK)) { PixelGrid.setPixel(4,1,ONCOLOR); }
  if (PixelGrid.wasPressed(PGButton::A)) { PixelGrid.setPixel(5,1,ONCOLOR); }
  if (PixelGrid.wasPressed(PGButton::B)) { PixelGrid.setPixel(6,1,ONCOLOR); }

  if (PixelGrid.wasReleased(PGButton::U)) { PixelGrid.setPixel(0,2,ONCOLOR); }
  if (PixelGrid.wasReleased(PGButton::R)) { PixelGrid.setPixel(1,2,ONCOLOR); }
  if (PixelGrid.wasReleased(PGButton::D)) { PixelGrid.setPixel(2,2,ONCOLOR); }
  if (PixelGrid.wasReleased(PGButton::L)) { PixelGrid.setPixel(3,2,ONCOLOR); }
  if (PixelGrid.wasReleased(PGButton::OK)) { PixelGrid.setPixel(4,2,ONCOLOR); }
  if (PixelGrid.wasReleased(PGButton::A)) { PixelGrid.setPixel(5,2,ONCOLOR); }
  if (PixelGrid.wasReleased(PGButton::B)) { PixelGrid.setPixel(6,2,ONCOLOR); }

  if (!digitalRead(8)){ PixelGrid.setPixel(0,3,ONCOLOR); }
  if (!digitalRead(3)){ PixelGrid.setPixel(1,3,ONCOLOR); }
  if (!digitalRead(4)){ PixelGrid.setPixel(2,3,ONCOLOR); }
  if (!digitalRead(18)){ PixelGrid.setPixel(3,3,ONCOLOR); }
  if (!digitalRead(41)){ PixelGrid.setPixel(4,3,ONCOLOR); }
  if (!digitalRead(17)){ PixelGrid.setPixel(5,3,ONCOLOR); }
  if (!digitalRead(42)){ PixelGrid.setPixel(6,3,ONCOLOR); }
  
  PixelGrid.update();
}
