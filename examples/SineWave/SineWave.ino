
#include "PixelGrid.h"

void setup() 
{
  SerialUSB.begin(115200);
  PGButtons::Setup();
  PGSounds::Setup();
  PGGraphics::Setup();
}

void sounds()
{
  if (PGButtons::Was_Pressed(PGButtons::A))
  {
    PGSounds::Play(PGSounds::Powerup);
  }
  if (PGButtons::Was_Pressed(PGButtons::B))
  {
    PGSounds::Play(PGSounds::Blip);
  }
  if (PGButtons::Was_Pressed(PGButtons::OK))
  {
  }
  if (PGButtons::Was_Pressed(PGButtons::U))
  {
  }
  if (PGButtons::Was_Pressed(PGButtons::D))
  {
  }
}



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


void my_program()
{
  start_r = inc_channel(start_r);
  start_g = inc_channel(start_g);
  start_g = inc_channel(start_g);
  start_b = inc_channel(start_b);
  bool flash = PGButtons::Is_Down(PGButtons::B);
  
  PGGraphics::Clear();
  phase++;
  for (int i=0; i<13; ++i)
  {
    float x = (float)i * spd + (float)phase * 0.4f;
    float y = 7.0f + amplitude * sin(x);
    int yval = (int)y;
    PGGraphics::SetPixel(i,yval,PGCOLOR(start_r>>4,start_g>>4,start_b>>4));
    
    int r2 = start_r-8;
    int g2 = start_g-8;
    int b2 = start_b-8;
    for (int c=yval+1;c<13; ++c)
    {
      PGGraphics::SetPixel(i,c,PGCOLOR(r2>>4,g2>>4,b2>>4));
      if (flash && (c == yval+2))
      {
        PGGraphics::SetPixel(i,c,PGCOLOR(r2>>2,g2>>1,b2>>2));
      }
      r2-=4;
      g2-=4;
      b2-=4;
      if (r2 < 0) r2 = 0;
      if (g2 < 0) g2 = 0;
      if (b2 < 0) b2 = 0;
    }
  }
  delay(10); // Pause before next pass through loop

  if (PGButtons::Was_Pressed(PGButtons::U)) amplitude += 0.2f;
  if (PGButtons::Was_Pressed(PGButtons::D)) amplitude -= 0.2f;
  if (PGButtons::Was_Pressed(PGButtons::L)) spd-=0.1f;
  if (PGButtons::Was_Pressed(PGButtons::R)) spd+=0.1f;
  if (PGButtons::Was_Pressed(PGButtons::OK)) { amplitude = INIT_AMP; spd = INIT_SPD; }
  if (PGButtons::Was_Pressed(PGButtons::A)) { start_r = random(min_val,max_val); start_g = random(min_val,max_val); start_b = random(min_val - max_val); }
  
  static int _pcounter =  0;
  for (int i=0; i<PGButtons::BCOUNT; ++i)
  {
    if (PGButtons::Was_Pressed((PGButtons::ButtonType)i)) {
      SerialUSB.print(i);
      _pcounter++;
    }
    if (_pcounter>50)
    {
      SerialUSB.println();
      _pcounter = 0;
    }
  }

  sounds();
}


void loop() 
{
  // Clear the screen, update your game logic, draw the new frame.
  my_program();

  // When you're done with the logic for a frame, let the system update
  PGButtons::Update();
  PGSounds::Update();
  PGGraphics::Update();
}