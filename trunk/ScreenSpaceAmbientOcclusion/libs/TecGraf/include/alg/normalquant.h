// Implements unit normal quantization
// fabraham@tecgraf.puc-rio.br
// Mar 2010
#ifndef ALG_NORMAL_QUANT_H
#define ALG_NORMAL_QUANT_H

#include "defines.h"
#include "vector.h"

/**
 * Packs unit normal 'vec' into an unsigned short value.
 */
unsigned short AlgUnitNormalPack (const AlgVector& vec);
/**
 * Unpacks an unit normal 'vec' stored in an unsigned short value.
 */
AlgVector AlgUnitNormalUnpack (unsigned short pvec);
/**
 * Initializes this module.
 */
void AlgUnitNormalInit ();
/**
 * Test function.
 */
bool AlgUnitNormalTest ();

#endif

