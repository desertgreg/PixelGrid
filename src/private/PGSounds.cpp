#include "PGSounds.h"


// Enable this and attach a probe to the UART RX pin to measure the diming and duty cycle of the sound interrupt
#define SCOPE_TIMING 01
#define SCOPE_TIMING_PIN 6

#define USE_ZEROTIMER 0

#if USE_ZEROTIMER
#define USING_TIMER_TC3 true      // Only TC3 can be used for SAMD51
#include "SAMDTimerInterrupt.h"

// Init selected SAMD timer
SAMDTimer SoundTimer(TIMER_TC3);

#endif



/////////////////////////////////////////////////////
// 
//  Sound Engine State
//
/////////////////////////////////////////////////////

uint8_t s_StreamingBuffer = 0;
uint8_t s_DmaBuffer0[4096];
uint8_t s_DmaBuffer1[4096];

struct ActiveSoundStruct
{
	const int8_t * m_Data = nullptr;
	uint32_t m_Size = 0;			// size in bytes of the sound
	uint32_t m_Cursor = 0;			// 8bits of fraciton
	uint8_t m_Active = 0;
	uint8_t m_Volume = 0xFF; 			// 0xFF = 1.0 
	PGSoundSampleRate m_SampleRate = PG_SAMPLERATE_4KHZ; // sampling rate of the data
	//uint8_t m_PlaybackRate = 0x80; 	// future, maybe allow for faster/slower playback (pitch changing)
	uint16_t m_PlaybackIncrement;  // 8bits of fraction, 32Khz clock
	uint8_t m_Loop = 0;
};

static const int SOUND_CHANNEL_COUNT = 2;
ActiveSoundStruct s_ActiveSounds[SOUND_CHANNEL_COUNT];


static void debug_channel(int c)
{
	SerialUSB.print("c[");
	SerialUSB.print(c);
	SerialUSB.print("] size=");
	SerialUSB.print(s_ActiveSounds[c].m_Size);
	SerialUSB.print(" cursor=");
	SerialUSB.print(s_ActiveSounds[c].m_Cursor);
	SerialUSB.print(" increment=");
	SerialUSB.print(s_ActiveSounds[c].m_PlaybackIncrement);
	SerialUSB.print("\r\n");
}
	


/////////////////////////////////////////////////////
// 
//  Sound Engine Implementation
//
/////////////////////////////////////////////////////
inline int16_t Update_Channel(ActiveSoundStruct & s)
{
	int16_t sample = s.m_Data[s.m_Cursor>>8];
	s.m_Cursor += s.m_PlaybackIncrement;
	if (s.m_Cursor>>8 >= s.m_Size)
	{
		s.m_Cursor = 0;
		if (s.m_Loop == 0)
		{
			s.m_Active = 0;
		}
	}
	
	sample = (sample * s.m_Volume) >> 8;
	return sample;
}

void Sound_Update()
{
#if SCOPE_TIMING
	digitalWrite(SCOPE_TIMING_PIN,1);
#endif

	int16_t accumulator = 0;
	
	for (int i=0; i<SOUND_CHANNEL_COUNT; ++i)
	{
		if (s_ActiveSounds[i].m_Active)
		{
			accumulator += Update_Channel(s_ActiveSounds[i]);
		}
	}

	// convert to 0..255, clamp and send to DAC
	int16_t sample = accumulator + 128;
	if (sample > 255) sample = 255;
	if (sample < 0) sample = 0;
	
	DAC->DATA.reg = (uint8_t)sample;
	
#if SCOPE_TIMING
	digitalWrite(SCOPE_TIMING_PIN,0);
#endif

}

// Interrupt handler for TC5
void TC5_Handler()
{
	//if (TC->INTFLAG.bit.MC0 == 1) {  // A compare to cc0 caused the interrupt
	//  TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag ovf flag
	//}

	TcCount16* TC = (TcCount16*) TC5; // get timer struct
	if (TC->INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt
	TC->INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
	}

	Sound_Update();
}


void PGSounds::setup()
{
	memset(s_DmaBuffer0,0,sizeof(s_DmaBuffer0));
	memset(s_DmaBuffer1,0,sizeof(s_DmaBuffer1));

#if USE_ZEROTIMER
	int period = 48000000/8000;
	SoundTimer.attachInterruptInterval_MS(1,Sound_Update);
#else 
	DAC->CTRLA.bit.ENABLE = 0x01;

	//First configure Generic Clock(GENCTRL related Setting)
	GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |  //50-50 Duty(Though,it will not reflect on any output pin)
					  GCLK_GENCTRL_GENEN |        //Enable generic clock generator
					  GCLK_GENCTRL_SRC_DFLL48M |  //48MHz clock PLL clock as the input
					  GCLK_GENCTRL_ID(4);         //Select GCLK 4
	while (GCLK->STATUS.bit.SYNCBUSY);

	//Configure Divisor of Input Clock for GENCTRL(GENDIV Related Settings)
	GCLK->GENDIV.reg = GCLK_GENDIV_DIV(3) |      //Clock Divisor, CPU=48Mhz so 48divisor=1Mhz  24divisor=2Mhz, 12divisor=4Mhz, CHOOSING 3->16Mhz timer
					 GCLK_GENDIV_ID(4);           //GCLK4

	//Now configure GCLK4 & Feed it's output to TC5(CLKCTRL related settings)
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |         // Enable Generic Clock
					  GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
					  GCLK_CLKCTRL_ID(GCM_TC4_TC5);  //Feed GCLK4 output to TC5

	TC5->COUNT16.CTRLA.reg = TC_CTRLA_PRESCSYNC_PRESC | // Overflow on prescaler clock
						   TC_CTRLA_MODE_COUNT16 |  //16 Bit Mode
						   TC_CTRLA_PRESCALER_DIV1 | //1024  | //1024 Prescaler
						   TC_CTRLA_WAVEGEN_MFRQ;  //Match Frequency Mode

	// If clock Divisor above is 48 (1Mhz clock) Then:
	// CC = 248 = 4.015KHz
	// CC = 200 = 4.975KHz
	// CC = 124 = 8.001KHz
	// CC = 61  = 16.130KHz
	// CC = 30  = 32.255KHz
	
	// We selected clock divisor of 3 (16Mhz clock) Then:
	// CC = 4000 = 3.999Khz
	// CC = 2000 = 8Khz
	// CC = 1000 = 16Khz
	// CC = 499 = 32Khz
	
	// I chose 8Khz playback because until I have some form of sound compression, anything
	// higher than that is too wasteful.  At 8kHz, you only get 13s of sound samples before
	// you run out of RAM!
	TC5->COUNT16.CC[0].reg = 2000; //499; //250;// Set the CC register
	while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

	TC5->COUNT16.CTRLA.bit.ENABLE = 1;  //Enable TC5
	while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

	NVIC_DisableIRQ(TC5_IRQn);
	NVIC_ClearPendingIRQ(TC5_IRQn);
	NVIC_SetPriority(TC5_IRQn, 0);
	NVIC_EnableIRQ(TC5_IRQn);

	TC5->COUNT16.INTENSET.bit.OVF = 1;
#endif
#if SCOPE_TIMING
	pinMode(SCOPE_TIMING_PIN,OUTPUT);
#endif
}

void PGSounds::update()
{
	// don't have to do anything here, everything is in the hardware timer interrupt
	//if (s_ActiveSounds[0].m_Active)
	//	debug_channel(0);
}

void PGSounds::setChannelVolume(uint8_t channel,uint8_t volume)
{
	if ((channel < 0) || (channel >= SOUND_CHANNEL_COUNT)) 
	{
		return;
	}
	s_ActiveSounds[channel].m_Volume = volume;
}

void PGSounds::play(PGSound & sound,uint8_t channel,uint8_t loop)
{
	//SerialUSB.print("play\r\n");
	if ((channel < 0) || (channel >= SOUND_CHANNEL_COUNT)) 
	{
		return;
	}
	
	s_ActiveSounds[channel].m_Data = sound.m_Data;
	s_ActiveSounds[channel].m_Size = sound.m_Size;
	s_ActiveSounds[channel].m_Cursor = 0;
	//s_ActiveSounds[channel].m_PlaybackRate = playback_rate;
	s_ActiveSounds[channel].m_Loop = loop;

	// TODO: could let the user select the playback rate of the sound engine and 
	// adjust these step values accordingly.  That might mean if they play a 32K sample
	// and the rate is lower, the system could skip samples.  For now, I'll just leave
	// the timer interrupt at 32Khz but it uses about 13% of the CPU time.
	static uint16_t s_Increments[] = 
	{
		255/8,	//PG_SAMPLERATE_1KHZ
		255/4,
		255/2,
		255,
		255*2,	//PG_SAMPLERATE_16KHZ
		255*4	//PG_SAMPLERATE_32KHZ
	};
	s_ActiveSounds[channel].m_PlaybackIncrement = s_Increments[sound.m_SampleRate];
	s_ActiveSounds[channel].m_Active = 1;
	
	//debug_channel(channel);
	
}



