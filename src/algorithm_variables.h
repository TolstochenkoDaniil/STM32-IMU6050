#ifndef ALGORITHM_VARIABLES_H
#define ALGORITHM_VARIABLES_H

#include "Global_types.h"
#include "algorithm_types.h"

extern FILE* infile;
extern FILE* output;
extern	long pos;
extern _SINT8 reading;
extern _SINT8 isSet;
extern _SINT8 alinement_set;
extern _SINT32 tmp;
extern _UINT16 check_sum;
extern _UINT16 cycle_counter;
extern _FLOAT32 bias_counter;
extern _FLOAT32 t;
extern _FLOAT32 K;

extern TPacket InPutData;
extern TCalibratedSensorsData Data, CalibData;
extern TOutAlgorithmData OutData;

extern _FLOAT32 acc_scale_coefs[3][3];
extern _FLOAT32 gyro_scale_coefs[3][3];
extern _FLOAT32 acc_bias[3];
extern _FLOAT32 gyro_bias[3];
extern _FLOAT32 orth_matrix[3][3];
extern _FLOAT32 cosine_matrix[3][3];
extern _FLOAT32 cosine_matrix_new[3][3];
extern _FLOAT32 gyro_corr[3][3];
extern _FLOAT32 gyro_matrix[3][3];
extern _TVector fN_m_s_s; 
extern _TVector bias_gyro;

#endif
