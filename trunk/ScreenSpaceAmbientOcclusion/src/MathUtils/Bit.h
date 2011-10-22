/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2011  
 *
 *  Bit Utils Library.
 */
#ifndef _BIT_H_
#define _BIT_H_

#include <iostream>
#include <string>

#define UINT_BIT_SIZE 32

class Bit
{
public:
   /**
   * Simple Constructor
   */
  Bit() {}

  /**
   * Return the number of '1' bits in the word
   */
  static unsigned int numberOfSetBits(unsigned int v);
  
  /**
   * Return the equivalent string of the word
   */
  static std::string getBitString(unsigned int v);

  /**
   * Convenience print bit with \n
   */
  static void printBitLine(unsigned int v);

  /**
   * Convenience print bit 
   */
  static void printBit(unsigned int v);

  /**
   * Verify if the n bit is One. Little Endian. 
   * isBitActiveLeftToRight(1100, 0) = TRUE
   * isBitActiveLeftToRight(1100, 3) = FALSE
   */
  static bool isBitActiveLeftToRight(unsigned int v, int index);

  /**
   * Verify if the n bit is One. Big Endian. 
   * isBitActiveLeftToRight(1100, 0) = FALSE
   * isBitActiveLeftToRight(1100, 3) = TRUE
   */
  static bool isBitActiveRightToLeft(unsigned int v, int index);
};

#endif

