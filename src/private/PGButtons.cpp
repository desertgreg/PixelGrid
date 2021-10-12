#include "PGButtons.h"
#include <stdint.h>
#include <Arduino.h>
#include <SamD.h>

struct ButtonStruct
{
  uint8_t m_CurState;
  uint8_t m_PrevState;
  uint8_t m_DebounceState;
  uint8_t m_DebounceTime;
  uint8_t m_Pin;

  ButtonStruct()
  {
    m_CurState = 0;
    m_PrevState = 0;
    m_DebounceState = 0;
    m_DebounceTime = 0;
    m_Pin = 0;
  }
  
  void update(uint8_t cur_time)
  {
    uint8_t new_state = !digitalRead(m_Pin);
    m_PrevState = m_CurState;  
    if (new_state != m_DebounceState)
    {
      m_DebounceTime = cur_time;
    }
    if (cur_time - m_DebounceTime > 4)
    {
      m_CurState = new_state;
    }
    m_DebounceState = new_state;
  }

  uint8_t wasPressed()
  {
    return m_CurState && !m_PrevState;
  }
  uint8_t wasReleased()
  {
    return !m_CurState && m_PrevState;
  }

  uint8_t isDown()
  {
    return m_CurState;
  }
};


ButtonStruct m_Buttons[PGButton::BCOUNT];


void PGButtons::setup()
{
  // set arduino pin assignments
  m_Buttons[U].m_Pin = 8;   // 11, PA06
  m_Buttons[R].m_Pin = 3;   //14, PA09
  m_Buttons[D].m_Pin = 4;   // 13 PA08  <-- normally used by EDBG?  OK!  (maybe we should get all the buttons on EDBG pins!)
  m_Buttons[L].m_Pin = 18;   //10, PA05    <-- normally used by arduino serial   NO
  m_Buttons[OK].m_Pin = 41; //12, PA07    <-- normalle used by arduino SPI      NO
  m_Buttons[A].m_Pin = 17;  //9, PA04  <-- normaly analog
  m_Buttons[B].m_Pin = 42;  // 4, PA03 <-- normally AREF

  // PA13 -> input
  //PORT->Group[PORTA].PINCFG[13].reg = (uint8_t)PORT_PINCFG_INEN;
  //PORT->Group[PORTA].DIRCLR.reg = (uint32_t)(1<<13);    // DIRCLR clears a bit
  
  for (int i=0; i<BCOUNT; ++i)
  {
    pinMode(m_Buttons[i].m_Pin,INPUT);
  }
  
  // update a few times to eat any inital presses
  update();
  update();
  update();
}


void PGButtons::update()
{
  uint8_t cur_time = millis();
  for (int i=0; i<BCOUNT; ++i)
  {
    m_Buttons[i].update(cur_time);
  }
}

uint8_t PGButtons::isDown(PGButton b)
{
  return m_Buttons[b].isDown();
}

uint8_t PGButtons::wasPressed(PGButton b)
{
  return m_Buttons[b].wasPressed();
}

uint8_t PGButtons::wasReleased(PGButton b)
{
  return m_Buttons[b].wasReleased();
}
