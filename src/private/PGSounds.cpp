#include "PGSounds.h"




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
	const uint8_t * m_Data = nullptr;
	uint32_t m_Size = 0;			// size in bytes of the sound
	uint32_t m_Cursor = 0;			// 7bits of fraciton
	uint8_t m_Active = 0;
	uint8_t m_Volume = 0xFF; 			// 0xFF = 1.0 
	uint8_t m_PlaybackRate = 0x80; 	// 0x80 = normal
	uint8_t m_Loop = 0;
};

static const int SOUND_CHANNEL_COUNT = 2;
ActiveSoundStruct s_ActiveSounds[SOUND_CHANNEL_COUNT];



/////////////////////////////////////////////////////
// 
//  Sound Engine Implementation
//
/////////////////////////////////////////////////////
inline uint8_t Update_Channel(ActiveSoundStruct & s)
{
	uint16_t sample = s.m_Data[s.m_Cursor>>7];
	s.m_Cursor += s.m_PlaybackRate;
	if (s.m_Cursor>>7 >= s.m_Size)
	{
		s.m_Cursor = 0;
		if (s.m_Loop == 0)
		{
			s.m_Active = 0;
		}
	}
	
	sample = (sample * s.m_Volume) >> 8;
	return (uint8_t)sample;
}

void TC5_Update()
{
	uint16_t accumulator = 0;
	
	for (int i=0; i<SOUND_CHANNEL_COUNT; ++i)
	{
		if (s_ActiveSounds[i].m_Active)
		{
			accumulator += Update_Channel(s_ActiveSounds[i]);
		}
	}

	DAC->DATA.reg = accumulator >> 1;  //Must change this if we change SOUND_CHANNEL_COUNT
}

// Interrupt handler for TC5
void TC5_Handler()
{
	//static int toggle;
	//toggle = !toggle;
	//digitalWrite(0,toggle);

	//if (TC->INTFLAG.bit.MC0 == 1) {  // A compare to cc0 caused the interrupt
	//  TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag ovf flag
	//}

	TcCount16* TC = (TcCount16*) TC5; // get timer struct
	if (TC->INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt
	TC->INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
	}

	TC5_Update();
}


void PGSounds::setup()
{
	memset(s_DmaBuffer0,0,sizeof(s_DmaBuffer0));
	memset(s_DmaBuffer1,0,sizeof(s_DmaBuffer1));
	DAC->CTRLA.bit.ENABLE = 0x01;

	//First configure Generic Clock(GENCTRL related Setting)
	GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |  //50-50 Duty(Though,it will not reflect on any output pin)
					  GCLK_GENCTRL_GENEN |        //Enable generic clock generator
					  GCLK_GENCTRL_SRC_DFLL48M |  //48MHz clock PLL clock as the input
					  GCLK_GENCTRL_ID(4);         //Select GCLK 4
	while (GCLK->STATUS.bit.SYNCBUSY);

	//Configure Divisor of Input Clock for GENCTRL(GENDIV Related Settings)
	GCLK->GENDIV.reg = GCLK_GENDIV_DIV(48) |         //Select clock divisor as 48 i.e GCLK4 output will be 48/48 = 1MHz
					 GCLK_GENDIV_ID(4);           //GCLK4

	//Now configure GCLK4 & Feed it's output to TC5(CLKCTRL related settings)
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |         // Enable Generic Clock
					  GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
					  GCLK_CLKCTRL_ID(GCM_TC4_TC5);  //Feed GCLK4 output to TC5

	TC5->COUNT16.CTRLA.reg = TC_CTRLA_PRESCSYNC_PRESC | // Overflow on prescaler clock
						   TC_CTRLA_MODE_COUNT16 |  //16 Bit Mode
						   TC_CTRLA_PRESCALER_DIV1 | //1024  | //1024 Prescaler
						   TC_CTRLA_WAVEGEN_MFRQ;  //Match Frequency Mode

	TC5->COUNT16.CC[0].reg = 200; //250;// Set the CC register
	while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

	TC5->COUNT16.CTRLA.bit.ENABLE = 1;  //Enable TC5
	while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

	NVIC_DisableIRQ(TC5_IRQn);
	NVIC_ClearPendingIRQ(TC5_IRQn);
	NVIC_SetPriority(TC5_IRQn, 0);
	NVIC_EnableIRQ(TC5_IRQn);

	TC5->COUNT16.INTENSET.bit.OVF = 1;
}

void PGSounds::update()
{
	// don't have to do anything here, everything is in the hardware timer interrupt
}

void PGSounds::setChannelPlaybackRate(uint8_t channel,uint8_t rate)
{
	if ((channel < 0) || (channel >= SOUND_CHANNEL_COUNT)) 
	{
		return;
	}
	s_ActiveSounds[channel].m_PlaybackRate = rate;
}

void PGSounds::setChannelVolume(uint8_t channel,uint8_t volume)
{
	if ((channel < 0) || (channel >= SOUND_CHANNEL_COUNT)) 
	{
		return;
	}
	s_ActiveSounds[channel].m_Volume = volume;
}

void PGSounds::play(PGSound & sound,uint8_t channel,uint8_t playback_rate, uint8_t loop)
{
	if ((channel < 0) || (channel >= SOUND_CHANNEL_COUNT)) 
	{
		return;
	}
	
	s_ActiveSounds[channel].m_Data = sound.m_Data;
	s_ActiveSounds[channel].m_Size = sound.m_Size;
	s_ActiveSounds[channel].m_Cursor = 0;
	s_ActiveSounds[channel].m_PlaybackRate = playback_rate;
	s_ActiveSounds[channel].m_Loop = loop;
	s_ActiveSounds[channel].m_Active = 1;
}


