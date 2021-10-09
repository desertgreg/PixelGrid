
#include <stdint.h>

class RamFile
{
public:
  RamFile();

  void open(const uint8_t * data, int size);
  void close();
  void read(uint8_t * buf,int size);
  bool available(); 
  
  const uint8_t * m_Data;
  int m_Size;
  int m_Pointer;
};
