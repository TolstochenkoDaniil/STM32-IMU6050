#ifndef GLOBAL_TYPES_H
#define GLOBAL_TYPES_H

#include "stdio.h"
// ---------------------------------------------------------------------------

#define _SINT8     signed char
#define _UINT8     unsigned char
#define _SINT16    signed short
#define _UINT16    unsigned short
#define _SINT32    signed long
#define _UINT32    unsigned long
#define _FLOAT32   float
#define _DOUBLE64  double

typedef _FLOAT32 _TVector[3];
typedef _FLOAT32 _TMatrix[3][3];
// ---------------------------------------------------------------------------

#endif
