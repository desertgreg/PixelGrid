#include "RamFile.h"
#include <string.h>

RamFile::RamFile() :
  m_Data(nullptr),
  m_Size(0),
  m_Pointer(0)
{
}

void RamFile::open(const uint8_t * data, int size)
{
  m_Data = data;
  m_Size = size;
  m_Pointer = 0;
}

void RamFile::close() 
{
  m_Data = nullptr;
  m_Size = 0;
  m_Pointer = 0;
}

void RamFile::read(uint8_t * buf,int size)
{
  int bytes_left = m_Size - m_Pointer;
  if (bytes_left < size)
  {
    memcpy(buf,&(m_Data[m_Pointer]),bytes_left);
    memset(buf + bytes_left,0,size - bytes_left);
    m_Pointer += bytes_left;  // should be equal to m_Size now.
  }
  else
  {
    memcpy(buf,&(m_Data[m_Pointer]),size);
    m_Pointer+= size;
  }
  for (int i=0; i<size; ++i)
  {
    buf[i] = buf[i] >> 4;
  }
  
}

bool RamFile::available() 
{
  return (m_Data != nullptr) && (m_Pointer < m_Size);
}
