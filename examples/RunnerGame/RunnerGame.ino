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


const int STATE_WAITING = 0;
const int STATE_PLAYING = 1;
const int STATE_LOST = 2;
const int STATE_COUNTING = 3;

int g_GameState = STATE_WAITING;
int g_GameStateCounter = 0;
int g_CrateCount = 0;
int g_ScoreCounter = 0;


bool checkCharacter();


/////////////////////////////////////////////////////
// Character
/////////////////////////////////////////////////////

const float JUMPVEL = -0.5f;
const float GROUND = 9.0f;

float cx = 3.0f;
float cy = GROUND;
float cyvel = 0.0f;
float cyaccel = 0.02f;


void debug_char()
{
  SerialUSB.print(cy);
  SerialUSB.print(" ");
  SerialUSB.print(cyvel);
  SerialUSB.print("\r\n");
}

void moveCharacter()
{
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
}


bool isEven(int value)
{
  return (value % 2) == 0;
}

void updateCharacter()
{
    digitalWrite(PIN_LED,PixelGrid.isDown(PGButton::B));

    if (g_GameState == STATE_PLAYING)
    {
      moveCharacter();
      if (checkCharacter())
      {
        g_GameState = STATE_LOST;
        g_GameStateCounter = 0;
      }
    }

    // when the player loses, we'll draw the player blinking and frozen in place 
    bool blink = false;
    if (g_GameState == STATE_LOST)
    {
      blink = isEven(g_GameStateCounter / 10);   
    }
    
    if (blink == false)
    {
      PixelGrid.drawImage(cx,cy,DudeImg);
    }
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

    bool isPastPlayer()
    {
      if (m_x < cx) return true;
      else return false;
    }
    void update()
    {
      if (isActive())
      {
        if (g_GameState == STATE_PLAYING)
        {
          bool was_past_player = isPastPlayer();
          m_x += m_vx;
          bool is_past_player = isPastPlayer();
          if ((was_past_player == false) && (is_past_player == true))
          {
            g_CrateCount++;
          }
        }
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

void resetCrates()
{
  for (int i=0; i<MAX_CRATES; ++i)
  {
    crates[i].m_x = -999.0f;
  }
}

void udpateCrates()
{
  for (int i=0; i<MAX_CRATES; ++i)
  {
    crates[i].update();
  }

  // is it time to spawn another crate?
  if (g_GameState == STATE_PLAYING)
  {
    crateTimer--;
    if (crateTimer <= 0)
    {
      spawn_crate();
      crateTimer = MIN_CRATE_TIME + rand() % RANDOM_CRATE_TIME;
    }
  }
}

bool checkCharacter()
{
    // check if we are colliding with a box...
    for (int i=0; i<MAX_CRATES; ++i)
    {
      if (crates[i].isActive())
      {
        if ((crates[i].m_x >= 3) && (crates[i].m_x <= 4))
        {
          if (cy >= GROUND-2)
          {
            // oops!  hit the box
            PixelGrid.playSound(MonsterHurtSnd);
            return true;
          }
        }
      }
    }
    return false;
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
  if (g_GameState == STATE_PLAYING)
  {
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
  g_GameStateCounter++;
  
  PixelGrid.clear();
  updateBackground();
  udpateCrates();
  updateCharacter();

  if (g_GameState == STATE_WAITING)
  {
    if (PixelGrid.anyPressed())
    {
      g_GameState = STATE_PLAYING;
      g_GameStateCounter = 0;
    }
  }
  else if (g_GameState == STATE_LOST)
  {
    if (g_GameStateCounter > 150)
    {
      g_GameState = STATE_COUNTING;
      g_GameStateCounter = 0;
      g_ScoreCounter = 0;
    }
  }
  else if (g_GameState == STATE_COUNTING)
  {
    if (g_ScoreCounter < g_CrateCount)
    {
      if (g_GameStateCounter > 10) 
      {
        g_ScoreCounter ++;
        g_GameStateCounter = 0;
      }
    }
    PixelGrid.drawNumber(1,0,g_ScoreCounter);
    if (PixelGrid.anyPressed())
    {
      resetCrates();
      g_GameState = STATE_PLAYING;
      g_GameStateCounter = 0;   
    }
  }

  // done with this frame!
  PixelGrid.update();
}
