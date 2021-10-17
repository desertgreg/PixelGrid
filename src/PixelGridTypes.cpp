#include "PixelGridTypes.h"
#include <arduino.h>

void PGScrollerClass::debugPrint()
{
	SerialUSB.print("p:"); SerialUSB.print(m_pos); SerialUSB.print(" dest:"); SerialUSB.print(m_destPos); SerialUSB.print("scrolling: ");SerialUSB.print(isScrolling()); SerialUSB.print("\r\n");
}
