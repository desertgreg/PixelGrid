#include "PixelGrid.h"
#include "MusicLoop.h"

void setup() {
  
  SerialUSB.begin(115200);
  PixelGrid.setup();

  PixelGrid.playSound(BaseBoostedSnd,1,0x7f,1);
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

int music_rate = 0x80 << 4;

void loop() {
SerialUSB.print("hi\r\n");
  bool moved = false;
  if (PixelGrid.wasPressed(PGButton::R)) { cursor++; moved = true;}
  if (PixelGrid.wasPressed(PGButton::L)) { cursor--; moved = true;}

  if (PixelGrid.isDown(PGButton::D))
  {
    if (music_rate > 10<<4) music_rate -= 1;
    PixelGrid.setChannelPlaybackRate(1,(uint8_t)(music_rate>>4));
  }
  if (PixelGrid.isDown(PGButton::U))
  {
    if (music_rate < 0xff<<4) music_rate += 1;
    PixelGrid.setChannelPlaybackRate(1,(uint8_t)(music_rate>>4));
  }
  
  cursor = wrap_sound(cursor);
  
  if (moved || PixelGrid.wasPressed(PGButton::A))
  {
    PixelGrid.playSound(*sounds[cursor]);
  }
  PixelGrid.update();
}
