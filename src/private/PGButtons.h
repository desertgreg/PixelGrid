#ifndef PGBUTTONS_H
#define PGBUTTONS_H

#include "..\PixelGridTypes.h"
#include <stdint.h>

class PGButtons
{
public:

  static void setup();
  static void update();
  static uint8_t isDown(PGButton b);
  static uint8_t wasPressed(PGButton b);
  static uint8_t wasReleased(PGButton b);

};

#endif //PGBUTTONS_H
