//
// This example demponstrates alpha blending and brightness.  4 circles are drawn using alpha-blending
// The neopixel display is very bright so under most circumstances the brightness needs to be set to around 16-20 (out of 255)
// 

#include "PixelGrid.h"
#include "RunnerGraphics.h"
#include "RunnerSounds.h"


// Lesson ideas
/*
1) building and running - blinky
2) if statement button - blink another pixel
3) variables x,y - draw a pixel, explain coordinates
4) bouncing dot
5) joystick updates x,y move a pixel around
6) draw bitmap instead of a pixel
7) sounds, play a sound whenever it bounces
8) function to update the ball
10) float variables, velocity, jumping character function
11) draw scrolling background function
12) obstacle, use a class
*/


void setup() {
  PixelGrid.setup();
}


/////////////////////////////////////////////////////
// Crates 2x2 pixel obstacles
/////////////////////////////////////////////////////
class CrateClass
{
  public:
    
    bool isActive()
    {
      return m_x > -10.0f;
    }
    
    void activate()
    {
      m_x = 15.0f;
      m_vx = -0.1f;
    }
    
    void update()
    {
      if (isActive())
      {
        m_x += m_vx;
        PixelGrid.drawImage(m_x,10,CrateImg);
      }
    }
  
    float m_x = -999.0f;
    float m_vx;
};

const int MAX_CRATES = 6;
CrateClass crates[MAX_CRATES];

const int MIN_CRATE_TIME = 50;
const int RANDOM_CRATE_TIME = 50;
int crateTimer = 300;

void spawn_crate()
{
  for (int i=0; i<MAX_CRATES; ++i)
  {
    if (!crates[i].isActive())
    {
      crates[i].activate();
      break;
    }
  }
}

void udpateCrates()
{
  for (int i=0; i<MAX_CRATES; ++i)
  {
    crates[i].update();
  }

  // is it time to spawn another crate?
  crateTimer--;
  if (crateTimer <= 0)
  {
    spawn_crate();
    crateTimer = MIN_CRATE_TIME + rand() % RANDOM_CRATE_TIME;
  }
}

/////////////////////////////////////////////////////
// Character
/////////////////////////////////////////////////////

float cx = 3.0f;
float cy = 0.0f;
float cyvel = 0.0f;
float cyaccel = 0.03f;

const float JUMPVEL = -0.5f;
const float GROUND = 9.0f;

void debug_char()
{
  SerialUSB.print(cy);
  SerialUSB.print(" ");
  SerialUSB.print(cyvel);
  SerialUSB.print("\r\n");
}
void updateCharacter()
{
    digitalWrite(PIN_LED,PixelGrid.isDown(PGButton::B));

    // physics
    if (cy < GROUND) 
    {
      cy += cyvel;
      cyvel += cyaccel;
    }
    else
    {
      cy = GROUND;

      // if the user is on the ground, they can jump!
      if (PixelGrid.wasPressed(PGButton::B))
      {
         cyvel = JUMPVEL;
         cy += cyvel;
         PixelGrid.playSound(JumpSnd);
        //digitalWrite(PIN_LED,1);
      }
    }
    
    PixelGrid.drawImage(cx,cy,DudeImg);
}


/////////////////////////////////////////////////////
// Background
/////////////////////////////////////////////////////
int layer0x = 0;
int layer1x = 0;
int layer2x = 0;
int layer0v = -64;
int layer1v = -16;
int layer2v = -8;

void updateBackground()
{
  bool print = false;
  if (1) { //PixelGrid.isDown(PGButton::R)) { 
    layer2x += layer2v;
    layer1x += layer1v;
    layer0x += layer0v;
    //print = true;
  }
  else if (PixelGrid.isDown(PGButton::L)) { 
    layer2x -= layer2v;
    layer1x -= layer1v;
    layer0x -= layer0v;
    //print = true;
  }

  PixelGrid.resetRenderStates();
  
  if (PixelGrid.isDown(PGButton::A)) PixelGrid.setBlendMode(PGBlendMode::OPAQUE);
  
  int x2 = layer2x>>8;
  int x1 = layer1x>>8;
  int x0 = layer0x>>8;
  if (print)
  {
    SerialUSB.print(x0);
    SerialUSB.print(" ");
  }
  x2 = x2 % MountainsFarImg.m_Width;
  x1 = x1 % MountainsNearImg.m_Width;
  x0 = x0 % GrassImg.m_Width;
  if (print)
  {
    SerialUSB.print(x0);
    SerialUSB.print("\r\n");
  }

  PixelGrid.setBlendMode(PGBlendMode::OPAQUE);
  PixelGrid.drawImage(0,0,SkyGradientImg);
  
  // Draw the layers
  PixelGrid.setBlendMode(PGBlendMode::ALPHA);
  PixelGrid.drawImage(x2,0,MountainsFarImg);
  if (x2 + MountainsFarImg.m_Width < 13) { PixelGrid.drawImage(x2 + MountainsFarImg.m_Width,0,MountainsFarImg); }
  if (x2 - MountainsFarImg.m_Width > 0) { PixelGrid.drawImage(x2 - MountainsFarImg.m_Width,0,MountainsFarImg); }

  PixelGrid.drawImage(x1,0,MountainsNearImg);
  if (x1 + MountainsNearImg.m_Width < 13) { PixelGrid.drawImage(x1 + MountainsNearImg.m_Width,0,MountainsNearImg); }
  if (x1 - MountainsNearImg.m_Width > 0) { PixelGrid.drawImage(x1 - MountainsNearImg.m_Width,0,MountainsNearImg); }
  
  PixelGrid.drawImage(x0,12,GrassImg);
  if (x0 + GrassImg.m_Width < 13) { PixelGrid.drawImage(x0 + GrassImg.m_Width,12,GrassImg); }
  if (x0 - GrassImg.m_Width > 0) { PixelGrid.drawImage(x0 - GrassImg.m_Width,12,GrassImg); }
}


/////////////////////////////////////////////////////
// Main loop
/////////////////////////////////////////////////////


void loop() 
{
  if (PixelGrid.isDown(PGButton::U)) 
  { 
    if (PixelGrid.wasPressed(PGButton::B)) 
    {
      SerialUSB.print("B! "); 
    } 
    SerialUSB.print("loop\r\n"); 
  }
  /*if (PixelGrid.wasPressed(PGButton::U)) { y = y-1; }
  if (PixelGrid.wasPressed(PGButton::D)) { y = y+1; }
  if (PixelGrid.wasPressed(PGButton::L)) { x = x-1; }
  if (PixelGrid.wasPressed(PGButton::R)) { x = x+1; }

  PixelGrid.setPixel(x,y,PGCOLOR(0x80,0x80,0xff));
  */
  
  //if (PixelGrid.wasPressed(PGButton::A)) { bright = 128; PixelGrid.playSound(MenuMoveSnd);SerialUSB.print(bright);}
  //if (PixelGrid.wasPressed(PGButton::B)) { bright = 9; PixelGrid.playSound(SwooshSnd); SerialUSB.print(bright);}

  PixelGrid.clear();
  updateBackground();
  updateCharacter();
  udpateCrates();

  // done with this frame!
  PixelGrid.update();
}
