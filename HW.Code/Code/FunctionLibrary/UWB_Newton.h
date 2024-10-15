#ifndef UWB_Newton
#define UWB_Newton

#ifdef __cplusplus
extern "C" {
#endif

#include<stdlib.h>
#include"UWB_Matrix.h"
#include"UWB_Newton.c"

float *newton_3b3_3(float guess[3], float anchors_location[3][3], float distance[3]);

#ifdef __cplusplus
}
#endif

#endif
