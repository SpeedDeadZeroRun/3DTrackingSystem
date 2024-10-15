#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define matrix_size 3

struct nC3{
    int anchor_count;
    int nC3_size;
    int *nC3_array;
};

struct nC3 *nC3_Creator(int anchor_count){
    struct nC3 *structure = (struct nC3*)malloc(sizeof(struct nC3));
    structure->anchor_count = anchor_count;
    structure->nC3_size = nC3(anchor_count);
    structure->nC3_array = (int *)malloc(sizeof(int)*(structure->nC3_size)*matrix_size);
    int cursor[] = {0, 1, 2};
    for(int i = 0; i < structure->nC3_size; i++){
        memcpy(structure->nC3_array+(matrix_size*i), cursor, sizeof(int)*matrix_size);
        cursor[0] = cursor[0]+1;
        if(cursor[0] == cursor[1]){
            cursor[1] = cursor[1]+1;
            cursor[0] = 0;
        }
        if(cursor[1] == cursor[2]){
            cursor[2] = cursor[2]+1;
            cursor[1] = 1;
        }
    }
    return structure;
}

void nC3_Destroyer(struct nC3 *structure){
    free(structure->nC3_array);
    free(structure);
}
