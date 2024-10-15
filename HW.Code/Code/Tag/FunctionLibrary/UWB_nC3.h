#ifndef UWB_nC3
#define UWB_nC3

#ifdef __cplusplus
extern "C" {
#endif

#include<stdlib.h>
#include"UWB_nC3.c"

struct nC3;
void nC3_Destroyer(struct nC3 *structure);
struct nC3 *nC3_Creator(int anchor_count);

#ifdef __cplusplus
}
#endif

#endif
