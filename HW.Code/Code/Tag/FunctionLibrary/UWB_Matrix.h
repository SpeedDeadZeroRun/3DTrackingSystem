#ifndef UWB_Matrix
#define UWB_Matrix

#ifdef __cplusplus
extern "C" {
#endif

#include<stdlib.h>
#include"UWB_Math.h"
#include"UWB_Matrix.c"

float *find_3b3_Inverse_Matrix(float input[3][3]);
float *matrix_3b3_3_Multiplication(float partial_array[3][3], float function_array[3]);

#ifdef __cplusplus
}
#endif

#endif
