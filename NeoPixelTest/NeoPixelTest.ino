#include <Adafruit_NeoPixel_ZeroDMA.h>
#include <bittable.h>
#include <pins.h>

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include "SamdAudio.h"
#include "PGButtons.h"
#include "PGSound.h"
#include "sounds.h"


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        12 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 13*13 

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
//Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel_ZeroDMA pixels(NUMPIXELS, PIN, NEO_GRB);

//SamdAudio AudioSystem;

#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels

void setup() {

  
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pinMode(0,OUTPUT); 
  pinMode(1,OUTPUT); 
  
  PGButtons::Init();
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  SerialUSB.begin(115200);

  const uint32_t sample_rate = 4096;
  const uint8_t channel_count = 4;
  const uint16_t buffer_size = 4096;
  //AudioSystem.begin(sample_rate,channel_count,buffer_size);


  PGSound::Init();
}

void fill()
{
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 10, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
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

void rainbow()
{
  pixels.clear();
  int i=0;
  
  int r = start_r;
  int g = start_g;
  int b = start_b;

  start_r = inc_channel(start_r);
  start_g = inc_channel(start_g);
  start_b = dec_channel(start_b);
  
  for (int y=0; y<13; ++y)  
  {
    for (int x=0; x<13; ++x)
    {
      pixels.setPixelColor(i, pixels.Color(r>>4,g>>4,b>>4));
      i++;
      r = inc_channel(r);      
      g = inc_channel(g);
      b = dec_channel(b);
    }
  }
  SerialUSB.print(start_r);
  SerialUSB.println();
  delay(50); // Pause before next pass through loop

  pixels.show();
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
  bool flash = PGButtons::Is_Down(PGButtons::B);
  
  pixels.clear();
  phase++;
  for (int i=0; i<13; ++i)
  {
    float x = (float)i * spd + (float)phase * 0.4f;
    float y = 7.0f + amplitude * sin(x);
    int yval = (int)y;
    pixels.setPixelColor(i + yval*13,pixels.Color(start_r>>4,start_g>>4,start_b>>4));
    
    int r2 = start_r-8;
    int g2 = start_g-8;
    int b2 = start_b-8;
    for (int c=yval+1;c<13; ++c)
    {
      pixels.setPixelColor(i+c*13,pixels.Color(r2>>4,g2>>4,b2>>4));
      if (flash && (c == yval+2))
      {
        pixels.setPixelColor(i+c*13,pixels.Color(r2>>2,g2>>1,b2>>2));
      }
      r2-=4;
      g2-=4;
      b2-=4;
      if (r2 < 0) r2 = 0;
      if (g2 < 0) g2 = 0;
      if (b2 < 0) b2 = 0;
    }
  }
  pixels.show();
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

void sounds()
{
  if (PGButtons::Was_Pressed(PGButtons::A))
  {
    //AudioSystem.play(g_ExplosionData, g_ExplosionSize, 0) ;
    PGSound::Play(&(g_ExplosionData[0]), g_ExplosionSize);
  }
  if (PGButtons::Was_Pressed(PGButtons::B))
  {
    PGSound::Play(&(g_PickupCoin7Data[0]),g_PickupCoin7Size);
  }
  if (PGButtons::Was_Pressed(PGButtons::OK))
  {
    PGSound::Play(&(g_Powerup19Data[0]),g_Powerup19Size);
  }
  if (PGButtons::Was_Pressed(PGButtons::U))
  {
    PGSound::Play(&(g_BlipSelect33Data[0]),g_BlipSelect33Size);
  }
  if (PGButtons::Was_Pressed(PGButtons::D))
  {
    PGSound::Play(&(g_Explosion23Data[0]),g_Explosion23Size);
  }
  
}


void loop() 
{
  //rainbow();
  sinewave();
  //sounds();
  PGButtons::Update();
  //PGSound::Update();
}
