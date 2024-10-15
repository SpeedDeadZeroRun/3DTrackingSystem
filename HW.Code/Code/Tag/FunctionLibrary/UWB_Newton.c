#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"UWB_Matrix.h"

#define matrix_size 3

float *newton_3b3_3(float guess[matrix_size], 
                    float anchors_location[matrix_size][matrix_size], 
                    float distance[matrix_size]
                    ){
    // function matrix
    float function_array[matrix_size] = {0};
    for(int i = 0; i < matrix_size; i++){
        float xmx = guess[0]-anchors_location[i][0];
        float ymy = guess[1]-anchors_location[i][1];
        float zmz = guess[2]-anchors_location[i][2];
        float r = distance[i];
        function_array[i] = (xmx*xmx)+(ymy*ymy)+(zmz*zmz)-(r*r);
    }

    // partial matrix
    float partial_array[matrix_size][matrix_size] = {0};
    for(int i = 0; i < matrix_size; i++)
        for(int j = 0; j < matrix_size; j++)
            partial_array[i][j] = 2*(guess[j]-anchors_location[i][j]);

    // inverse partial matrix
    float *Inverse_Array_P = find_3b3_Inverse_Matrix(partial_array);
    float Inverse_Array[matrix_size][matrix_size] = {0};
    memcpy(Inverse_Array, Inverse_Array_P, sizeof(float)*matrix_size*matrix_size);
    free(Inverse_Array_P);

    // drifferent
    float *current_drifferent = matrix_3b3_3_Multiplication(Inverse_Array, function_array);

    // offset
    float *current_round = (float *)malloc(sizeof(float)*matrix_size);
    for(int i = 0; i < matrix_size; i++)
        *(current_round+i) = guess[i]-*(current_drifferent+i);
    free(current_drifferent);

    return current_round;
}