#ifndef PGBUTTONS_H
#define PGBUTTONS_H


#include <stdint.h>

class PGButtons
{
public:

  enum ButtonType
  {
    U = 0,
    R,
    D,
    L,
    OK,
    A,
    B,

    BCOUNT
  };
  
  static void Setup();
  static void Update();
  static uint8_t Is_Down(ButtonType b);
  static uint8_t Was_Pressed(ButtonType b);
  static uint8_t Was_Released(ButtonType b);

};

#endif //PGBUTTONS_H
