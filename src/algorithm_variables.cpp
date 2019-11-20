#include "algorithm_variables.h"
#include "Global_types.h"
#include "algorithm_configuration_calib.h"

FILE* infile;
FILE* output;
long pos;
_SINT8 reading = 0;
_SINT8 isSet = 0;
_SINT8 alinement_set = 0;
_SINT32 tmp;
_UINT16 check_sum = 0;
_UINT16 cycle_counter = 0;
_FLOAT32 bias_counter = 0.0;
_FLOAT32 t = 0.0;
_FLOAT32 K;

_FLOAT32 acc_scale_coefs[3][3] = 
{
	{ AXX, AXY, AXZ },
	{ AYX, AYY, AYZ },
	{ AZX, AZY, AZZ }
};

_FLOAT32 gyro_scale_coefs[3][3] = 
{
	{ BXX, BXY, BXZ },
	{ BYX, BYY, BYZ },
	{ BZX, BZY, BZZ }
};

_FLOAT32 acc_bias[3] = { AX, AY, AZ };

_FLOAT32 gyro_bias[3] = { BX, BY, BZ };

_FLOAT32 orth_matrix[3][3] = 
{
	{ ABXX, ABXY, ABXZ },
	{ ABYX, ABYY, ABYZ },
	{ ABZX, ABZY, ABZZ }
};

TCalibratedSensorsData Data, CalibData;
TPacket InPutData;
TOutAlgorithmData OutData;

_TVector fN_m_s_s = {0, 0, 0};
_TVector bias_gyro = {0, 0, 0};
_FLOAT32 cosine_matrix[3][3];
_FLOAT32 cosine_matrix_new[3][3];
_FLOAT32 gyro_matrix[3][3];
_FLOAT32 gyro_corr[3][3] = 
{
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 }
};

