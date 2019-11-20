#ifndef ALGORITHM_CONFIGURATION_H
#define ALGORITHM_CONFIGURATION_H

// Common variables
#define G 9.81
#define U 0.0041667
#define PHI 0.972
#define DEG_TO_RAD 0.017
#define RAD_TO_DEG 57.295
#define FREQ 100
#define PI 3.1415
#define dt 0.01

// Acc biases
#define AX 0.24258
#define AY 0.0383771
#define AZ -0.989191

// Gyro's biases
#define BX -0.0195371
#define BY -0.732907
#define BZ -0.661269

// Acc scale coefs
#define AXX 0.991022
#define AXY -0.000857093
#define AXZ 0.00515287
#define AYX 0.0043477
#define AYY 1.00074
#define AYZ -0.00179702
#define AZX 0.000195062
#define AZY 0.00831296
#define AZZ 0.990593

// Gyro scale coefs
#define BXX -0.248595
#define BXY 0.000729497
#define BXZ 0.00189861
#define BYX -0.000291941
#define BYY -0.249582
#define BYZ 0.00380139
#define BZX 0.00252339
#define BZY 0.000932706
#define BZZ -0.249532

// non-orthogonal coefs
#define ABXX -0.000617084
#define ABXY -0.0000487571
#define ABXZ -0.000526511
#define ABYX -0.000199513
#define ABYY -0.000522542
#define ABYZ -0.000288197
#define ABZX -0.000424186
#define ABZY -0.000120295
#define ABZZ -0.00285877

#endif
