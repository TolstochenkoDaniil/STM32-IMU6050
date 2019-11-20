#ifndef ALGORITHM_PROCEDURES_H
#define ALGORITHM_PROCEDURES_H

#include "Global_types.h"
#include "algorithm_variables.h"

extern _UINT16 crc16(_UINT8 *pcBlock, _UINT16 length);
extern void parse_data(void);
extern void matrix_set(TCalibratedSensorsData* Data);
extern void coef_application(void);
extern void sensor_alinement(void);
extern void puasson_equation(void);
extern void normalization(void);
extern void calculation(void);
extern void cosine_matrix_application(void);
extern void radial_correction(void);

#endif
