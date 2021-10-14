#include "PixelGrid.h"

void setup() {
  PixelGrid.setup();
}

int x_fp = 6<<3;
int y_fp = 6<<3;
int x = 6;
int y = 6;
int cursor_speed = 2;

////////////////////////////////////////////////////////
// X-Y Cursor
////////////////////////////////////////////////////////
void update_cursor()
{
  // Use the buttons to move x,y around the screen
  if (PixelGrid.isDown(PGButton::U))
  {
    y_fp=y_fp+cursor_speed;
  }
  if (PGButtons::isDown(PGButton::D))
  {
    y_fp=y_fp-cursor_speed;
  }
  if (PGButtons::isDown(PGButton::L))
  {
    x_fp=x_fp+cursor_speed;
  }
  if (PGButtons::isDown(PGButton::R))
  {
    x_fp=x_fp-cursor_speed;
  }
  x = x_fp>>3;
  y = y_fp>>3;
}


////////////////////////////////////////////////////////
// Sounds
////////////////////////////////////////////////////////
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

void test_sounds()
{
  bool moved = false;
  if (PixelGrid.wasPressed(PGButton::R)) { cursor++; moved = true;}
  if (PixelGrid.wasPressed(PGButton::L)) { cursor--; moved = true;}
  cursor = wrap_sound(cursor);
  
  if (moved || PixelGrid.wasPressed(PGButton::A))
  {
    PixelGrid.playSound(*sounds[cursor]);
  }
  PixelGrid.fill(PGCOLOR(0,0,10));//0,0,0x00001000); //PGCOLOR(10,0,0));
}

////////////////////////////////////////////////////////
// Images
////////////////////////////////////////////////////////
void test_images()
{
  PixelGrid.clear();
  PixelGrid.drawImage(x-10,y-10,CharacterTestImg);
}


////////////////////////////////////////////////////////
// Sinewave
////////////////////////////////////////////////////////
int min_val = 2<<4;
int max_val = 8<<4;

int start_r = 5<<4;
int start_g = 2<<4;
int start_b = 6<<4;

int inc_channel(int val)
{
  val++;
  if (val > max_val) val = min_val;
  return val;
}

int dec_channel(int val)
{
  val--;
  if (val < min_val) val = max_val;
  return val;
}

int phase = 0;
const float INIT_AMP = 6.0f;
const float INIT_SPD = 2.5f / 13.0f;
float amplitude =  INIT_AMP;
float spd = INIT_SPD;


void sinewave()
{
  start_r = inc_channel(start_r);
  start_g = inc_channel(start_g);
  start_g = inc_channel(start_g);
  start_b = inc_channel(start_b);
  bool flash = PixelGrid.isDown(PGButton::B);
  
  PixelGrid.clear();
  phase++;
  for (int i=0; i<13; ++i)
  {
    float x = (float)i * spd + (float)phase * 0.15f;
    float y = 7.0f + amplitude * sin(x);
    int yval = (int)y;
    PixelGrid.setPixel(i,yval,PGCOLOR(start_r>>4,start_g>>4,start_b>>4));
    
    int r2 = start_r-8;
    int g2 = start_g-8;
    int b2 = start_b-8;
    for (int c=yval+1;c<13; ++c)
    {
      PixelGrid.setPixel(i,c,PGCOLOR(r2>>4,g2>>4,b2>>4));
      if (flash && (c == yval+2))
      {
        PixelGrid.setPixel(i,c,PGCOLOR(r2>>2,g2>>1,b2>>2));
      }
      r2-=4;
      g2-=4;
      b2-=4;
      if (r2 < 0) r2 = 0;
      if (g2 < 0) g2 = 0;
      if (b2 < 0) b2 = 0;
    }
  }
  PixelGrid.drawImage(-x,-y,PixelGridImg);
  
  if (PixelGrid.wasPressed(PGButton::U)) amplitude += 0.2f;
  if (PixelGrid.wasPressed(PGButton::D)) amplitude -= 0.2f;
  if (PixelGrid.wasPressed(PGButton::L)) spd-=0.1f;
  if (PixelGrid.wasPressed(PGButton::R)) spd+=0.1f;
  if (PixelGrid.wasPressed(PGButton::OK)) { amplitude = INIT_AMP; spd = INIT_SPD; }
  if (PixelGrid.wasPressed(PGButton::A)) { start_r = random(min_val,max_val); start_g = random(min_val,max_val); start_b = random(min_val - max_val); }
}

////////////////////////////////////////////////////////
// bouncers
////////////////////////////////////////////////////////
int random_coord() { return (rand() % 13) << 4; }
int random_vel() { return (rand() % 7); }
pgcolor random_color() { int r=rand() &15; int g=rand()&15; int b=rand()&15; return PGCOLOR(r,g,b); }

struct bouncer
{
  int x;
  int y;
  int vx;
  int vy;
  pgcolor color;

  bouncer() { x = random_coord(); y=random_coord(); vx = random_vel(); vy=random_vel(); color = random_color(); } //PGCOLOR(10,10,12); }
  void update() { 
    x+=vx; y+=vy; if (x < 0) { vx = abs(vx); } if (y < 0) { vy = abs(vy); } if (x > 13<<4) { vx = -abs(vx); } if(y > 13<<4) {vy = -abs(vy); }
    PixelGrid.setPixel(x>>4,y>>4,color);
  }
};
const int BOUNCER_COUNT = 30;
bouncer m_Bouncers[BOUNCER_COUNT];

void bouncers()
{
  PixelGrid.clear();
  for (int i=0; i<BOUNCER_COUNT; ++i)
  {
    m_Bouncers[i].update();
  }
}


////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////

int mode = 0;
const int MODE_COUNT = 4;

void loop() {
  update_cursor();
  if (PixelGrid.wasPressed(PGButton::OK))
  {
    mode = (mode+1) % MODE_COUNT;
    x_fp = 6<<3;
    y_fp = 6<<3;
    PixelGrid.playSound(BossAppearSnd);
    PixelGrid.clear();
  }
  
  switch(mode)
  {
    case 0: test_sounds(); break;
    case 1: test_images(); break;
    case 2: sinewave(); break;
    case 3: bouncers(); break;
  }
  
  PixelGrid.update();
}
