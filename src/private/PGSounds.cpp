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
	const uint8_t * m_Data;
	uint16_t m_Size;
	uint16_t m_Cursor;
};

ActiveSoundStruct s_ActiveSound;



/////////////////////////////////////////////////////
// 
//  Sound Engine Implementation
//
/////////////////////////////////////////////////////

void TC5_Update()
{
	if (s_ActiveSound.m_Cursor < s_ActiveSound.m_Size)
	{
		s_ActiveSound.m_Cursor++;
		DAC->DATA.reg = s_ActiveSound.m_Data[s_ActiveSound.m_Cursor];
	}
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

void PGSounds::play(PGSound & sound)
{
	s_ActiveSound.m_Data = sound.m_Data;
	s_ActiveSound.m_Size = sound.m_Size;
	s_ActiveSound.m_Cursor = 0;
}

