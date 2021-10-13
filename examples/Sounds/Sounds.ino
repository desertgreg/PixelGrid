#include "PixelGrid.h"

void setup() {
  
  SerialUSB.begin(115200);
  PixelGrid.setup();
  
}

PGSound * sounds[] = 
{
  &BossAppearSnd,
  &EnterProgramSnd,
  &ExitProgramSnd,
  &MenuMoveSnd,
  &MonsterAppearSnd,
  &MonsterHurtSnd,
  &NextRoomSnd,
  &PickupCoinSnd,
  &PixelgridStartupSnd,
  &SwooshSnd,
  &TakeDamageSnd,
  &TestSoundSnd
};
const int SND_COUNT = sizeof(sounds)/sizeof(sounds[0]);
int cursor = 0;

int wrap_sound(int val)
{
  if (val < 0) val = SND_COUNT-1;
  if (val > SND_COUNT-1) val = 0;
  return val;
}

void loop() {

  bool moved = false;
  if (PixelGrid.wasPressed(PGButton::R)) { cursor++; moved = true;}
  if (PixelGrid.wasPressed(PGButton::L)) { cursor--; moved = true;}
  cursor = wrap_sound(cursor);
  
  if (moved || PixelGrid.wasPressed(PGButton::A))
  {
    PixelGrid.playSound(*sounds[cursor]);
  }
  PixelGrid.update();
}
