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
	{ 0.991022, -0.000857093, 0.00515287 },
	{ 0.0043477, 1.00074, -0.00179702 },
	{ 0.000195062, 0.00831296, 0.990593 }
};
//{
//	{ AXX, AXY, AXZ },
//	{ AYX, AYY, AYZ },
//	{ AZX, AZY, AZZ }
//};
_FLOAT32 gyro_scale_coefs[3][3] = 
{
	{ -0.248595, 0.000729497, 0.00189861 },
	{ -0.000291941, -0.249582, 0.00380139 },
	{ 0.00252339, 0.000932706, -0.249532 }
};
//{
//	{ BXX, BXY, BXZ },
//	{ BYX, BYY, BYZ },
//	{ BZX, BZY, BZZ }
//};
_FLOAT32 acc_bias[3] = 
{ 0.24258, 0.0383771, -0.989191 };
//{ AX, AY, AZ };
_FLOAT32 gyro_bias[3] = 
{ -0.0195371, -0.732907, -0.661269 };
//{ BX, BY, BZ };
_FLOAT32 orth_matrix[3][3] = 
{
	{ -0.000617084, -0.0000487571, -0.000526511 },
	{ -0.000199513, -0.000522542, -0.000288197 },
	{ -0.000424186, -0.000120295, -0.00285877 }
};
//{
//	{ ABXX, ABXY, ABXZ },
//	{ ABYX, ABYY, ABYZ },
//	{ ABZX, ABZY, ABZZ }
//};

TCalibratedSensorsData Data, CalibData;
TPacket InPutData;
TOutAlgorithmData OutData;

_TVector fN_m_s_s = {0, 0, 0};
_TVector bias_gyro = {0, 0, 0};
_FLOAT32 cosine_matrix[3][3];
_FLOAT32 cosine_matrix_new[3][3];
_FLOAT32 gyro_matrix[3][3];
_FLOAT32 gyro_corr[3][3] = {
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 }
};

