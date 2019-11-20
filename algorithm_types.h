#ifndef ALGORITHM_TYPES_H
#define ALGORITHM_TYPES_H

#include "Global_types.h"

// -----------------------------------------------------------------------------------------------------------
#pragma pack(1)
typedef struct
{
  _FLOAT32 OmegaB_deg_s[3];
  _FLOAT32 fB_m_s_s[3];
  _FLOAT32 Temperature_C;
} TCalibratedSensorsData;
#pragma pack()

// -----------------------------------------------------------------------------------------------------------

#pragma pack(1)
typedef struct
{
	_UINT16  Header;
	_UINT16  Flag;
	_UINT32  Index;
    _FLOAT32 OmegaB_deg_s[3];
	_FLOAT32 fB_m_s_s[3];
	_FLOAT32 Temperature_C;
    _FLOAT32 Roll_deg;
    _FLOAT32 Pitch_deg;
    _FLOAT32 Heading_deg;
    _UINT16  Reserved;
	_UINT16  Check_Sum;
} TPacket;
#pragma pack()

// -----------------------------------------------------------------------------------------------------------

#pragma pack(1)
typedef struct
{
  //_UINT32 Flags;
  _FLOAT32 Roll_deg;
  _FLOAT32 Pitch_deg;
  _FLOAT32 Heading_deg;
  //_FLOAT32 MagHeading_deg;
  //_FLOAT32 Speed_m_s;
  //_FLOAT32 Altitude_m;
  //_DOUBLE64 Lat_deg, Lon_deg;
} TOutAlgorithmData;
#pragma pack()

// -----------------------------------------------------------------------------------------------------------

#endif
