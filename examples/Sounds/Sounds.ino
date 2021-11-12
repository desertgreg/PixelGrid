#include "PixelGrid.h"
#include "MusicLoops.h"


PGSound music[] = 
{
  BaseBoostedASnd,
  BaseBoostedBSnd,
  BaseBoostedCSnd,
  BaseBoostedDSnd
};
const int MUSIC_COUNT = sizeof(music)/sizeof(music[0]);
int music_cursor = 0;

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


void setup() {
  
  SerialUSB.begin(115200);
  PixelGrid.setup();
  //PixelGrid.playSound(music[music_cursor],1,1);
}


int wrap_sound(int val)
{
  if (val < 0) val = SND_COUNT-1;
  if (val > SND_COUNT-1) val = 0;
  return val;
}

int wrap_music(int val)
{
  if (val < 0) val = MUSIC_COUNT-1;
  if (val > MUSIC_COUNT-1) val = 0;
  return val;
}

int music_rate = 0x80 << 4;

void loop() {

  PixelGrid.clear();
  bool moved = false;
  bool music_changed = false;

  /////////////////////////////////////////////////////////////////////
  // SOUNDS
  // Left right move through sound effects
  if (PixelGrid.wasPressed(PGButton::R)) { cursor++; moved = true;}
  if (PixelGrid.wasPressed(PGButton::L)) { cursor--; moved = true;}
  cursor = wrap_sound(cursor);

  if (moved || PixelGrid.wasPressed(PGButton::A))
  {
    PixelGrid.setPixel(5,5,PGCOLOR(0,0,100));
    PixelGrid.playSound(*sounds[cursor]);
    //SerialUSB.print(cursor);
  }


  /////////////////////////////////////////////////////////////////////
  // MUSIC at different sample rates
  // Up down raise and lower the sample rate of the music loop
  if (PixelGrid.wasPressed(PGButton::D))
  {
    music_cursor--;
    music_changed = true;
  }
  if (PixelGrid.wasPressed(PGButton::U))
  {
    music_cursor++;
    music_changed = true;
  }
  music_cursor = wrap_music(music_cursor);
  
  if (music_changed)
  {
      PixelGrid.playSound(music[music_cursor],1,1);
  }

  PixelGrid.setPixel(0,music_cursor,PGCOLOR(100,0,0));
  PixelGrid.setPixel(2,cursor,PGCOLOR(0,100,0));

  // Always let the system update once per 'loop'
  PixelGrid.update();
}
