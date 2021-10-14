#include "PGFrameBuffer.h"


// NOTE - got these pin numbers by 'printing' them from a sketch
// PIN_SPI_MOSI = 23
// PIN_SPI_MISO = 22		// TODO, on PixelGrid, I think I have these connected to SCL/SDA?
// PIN_SPI_SCK = 24
// PIN_LED = 13
// SDA = 20					
// SCL = 21
// Serial1 is on 0(RX), 1(TX)
// Arduino Zero default SPI is on port pins: PB11 (SCK), PB10 (MOSI) and PA12 (MISO)
// In Arduino Zero's Schematic there is two ports with I2C communication, D3/D4(PA09/PA08) e I2C_SDA/I2C_SCL(PA22/PA23).


void PGFrameBuffer::begin()
{
	pinMode(NEOPIXEL_PIN, OUTPUT);
    digitalWrite(NEOPIXEL_PIN, LOW);
	
	SerialUSB.print(FB_SIZE);
	SerialUSB.print("\r\n");
	
	memset(m_FrontBuffer,0,sizeof(m_FrontBuffer));
    m_Spi = new SPIClass(&sercom1, NEOPIXEL_PIN, NEOPIXEL_PIN, NEOPIXEL_PIN, SPI_PAD_3_SCK_1, SERCOM_RX_PAD_1);
	m_Spi->begin();
	pinPeripheral(NEOPIXEL_PIN, PIO_SERCOM);
	m_Dma.setTrigger(SERCOM1_DMAC_ID_TX);
	m_Dma.setAction(DMA_TRIGGER_ACTON_BEAT);
	if (DMA_STATUS_OK == m_Dma.allocate())
	{
		if (m_Dma.addDescriptor(m_FrontBuffer,(void*)(&SERCOM1->SPI.DATA.reg),FB_SIZE,DMA_BEAT_SIZE_BYTE,true,false))
		{
			m_Dma.loop(true);
			memset(m_FrontBuffer,0,FB_SIZE);
			m_Spi->beginTransaction(SPISettings(2400000,MSBFIRST,SPI_MODE0));
			m_Dma.startJob();
		}
	}
}

void PGFrameBuffer::setBrightness(uint8_t val)
{
	m_Brightness = val + 1;  // 255 brightness -> store as 256 (1.0 in 8.8 fixed point)
}

void PGFrameBuffer::setPixelColor(int addr, pgcolor color)
{
	m_BackBuffer[addr] = color;
}

pgcolor * PGFrameBuffer::getBackBuffer()
{
	return &(m_BackBuffer[0]);
}

uint32_t PGFrameBuffer::expand_byte(uint8_t byte)
{
	uint32_t expanded = 0;
	
	// apply brightness
	byte = (byte * m_Brightness)>>8;
	
	// expand every bit into 3 bits.  each 0 becomes 100 and each 1 becomes 110 (algorithm from adafruit)
	uint8_t abef = byte & 0b11001100;
	uint8_t cdgh = byte & 0b00110011;
	expanded = ((abef * 0b1010000010100000) & 0b010010000000010010000000) | ((cdgh * 0b0000101000001010) & 0b000000010010000000010010) | 0b100100100100100100100100;

	return expanded;
}

void PGFrameBuffer::show()
{
	// Copy the back buffer to the front buffer, expanding each bit 3:1 for the neopixel timing
	// Pixels are in the form a,r,g,b so we need to skip each 'a' byte as we convert.
	uint8_t *in = (uint8_t*)m_BackBuffer;
	uint8_t *out = m_FrontBuffer;
	uint32_t expand;
	for (int i=0; i<NUMPIXELS; i++) 
	{
		uint8_t a,r,g,b;
		a = *in++;  // alpha gets ignored

		r = *in++;
		expand = expand_byte(r);
		*out++ = expand>>16;
		*out++ = expand>>8;
		*out++ = expand;

		g = *in++;
		expand = expand_byte(g);
		*out++ = expand>>16;
		*out++ = expand>>8;
		*out++ = expand;
		
		b = *in++;
		expand = expand_byte(b);
		*out++ = expand>>16;
		*out++ = expand>>8;
		*out++ = expand;
	}
}
