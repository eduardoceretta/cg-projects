/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2011  
 *
 *  Bit Utils Library.
 */

#include "Bit.h"
#include <assert.h>


std::string Bit::getBitString(unsigned int v)
{
  char s[UINT_BIT_SIZE + 1];
  for(int i = UINT_BIT_SIZE - 1; i >= 0; --i)
  {
    unsigned mask = 1 << i;
    s[UINT_BIT_SIZE - 1 - i] = v & mask ? '1' : '0';
  }
  s[UINT_BIT_SIZE] = '\0';
  return std::string(s);
}
void Bit::printBitLine(unsigned int v)
{
  printf(getBitString(v).c_str());
}

void Bit::printBit(unsigned int v)
{
  printf("%s\n",getBitString(v).c_str());
}


bool Bit::isBitActiveLeftToRight(unsigned int v, int index)
{
  unsigned i = 1 << (UINT_BIT_SIZE - 1 - index);
  return v&i;
}

bool Bit::isBitActiveRightToLeft(unsigned int v, int index)
{
  unsigned i = 1 << (index);
  return v&i;
}

unsigned int Bit::numberOfSetBits(unsigned int i)
{
  assert(UINT_BIT_SIZE == 32); //Only Works for 32 bits
  i = i - ((i >> 1) & 0x55555555);
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}


