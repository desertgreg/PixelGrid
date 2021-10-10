#include "PixelGrid.h"

void setup() {
  
  SerialUSB.begin(115200);
  PixelGrid::Setup();
  
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
  if (PGButtons::Was_Pressed(PGButtons::R)) { cursor++; moved = true;}
  if (PGButtons::Was_Pressed(PGButtons::L)) { cursor--; moved = true;}
  cursor = wrap_sound(cursor);
  
  if (moved || PGButtons::Was_Pressed(PGButtons::A))
  {
    PGSounds::Play(*sounds[cursor]);
  }
  PixelGrid::Update();
}
