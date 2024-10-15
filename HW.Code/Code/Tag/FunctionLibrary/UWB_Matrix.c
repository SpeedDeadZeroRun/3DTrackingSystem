#include<stdlib.h>
#include<stdio.h>
#include"UWB_Math.h"

#define matrix_size 3

float *find_3b3_Inverse_Matrix(float input[matrix_size][matrix_size]){
    // determinant
    float det = 0.0;
    for(int i = 0; i < matrix_size; i++){
        int im = (i)%matrix_size;
        int iom = (i+1)%matrix_size;
        int itm = (i+2)%matrix_size;
        det = det+(
            (input[0][im]*input[1][iom]*input[2][itm])-
            (input[2][im]*input[1][iom]*input[0][itm])
        );
    }
    if(det == 0.0) det = 0.01;

    // transpose
    float transpose_array[matrix_size][matrix_size] = {0};
    for(int i = 0; i < matrix_size; i++)
        for(int j = 0; j < matrix_size; j++)
            transpose_array[i][j] = input[j][i];

    // determinant
    float determinant_array[matrix_size][matrix_size] = {0};
    for(int i = 0; i < matrix_size; i++)
        for(int j = 0; j < matrix_size; j++){
            int iom = (i+1)%matrix_size;
            int itm = (i+2)%matrix_size;
            int jom = (j+1)%matrix_size;
            int jtm = (j+2)%matrix_size;
            int il = iom;
            int ih = itm;
            int jl = jom;
            int jh = jtm;
            if(il > ih){
                il = itm;
                ih = iom;
            }
            if(jl > jh){
                jl = jtm;
                jh = jom;
            }
            determinant_array[i][j] = (
                (transpose_array[il][jl]*transpose_array[ih][jh])-
                (transpose_array[ih][jl]*transpose_array[il][jh])
            );                      
        }

    // cofactors
    determinant_array[0][1] = -determinant_array[0][1];
    determinant_array[1][0] = -determinant_array[1][0];
    determinant_array[1][2] = -determinant_array[1][2];
    determinant_array[2][1] = -determinant_array[2][1];

    // divide
    float *divided_array = (float *)malloc(sizeof(float)*matrix_size*matrix_size);
    for(int i = 0; i < matrix_size; i++)
        for(int j = 0; j < matrix_size; j++)
            *(divided_array+(i*matrix_size)+j) = determinant_array[i][j]/det;

    return divided_array;
}

float *matrix_3b3_3_Multiplication(float partial_array[matrix_size][matrix_size], float function_array[matrix_size]){
    float *drifferent_array = (float *)malloc(sizeof(float)*matrix_size);
    for(int i = 0; i < matrix_size; i++)
        *(drifferent_array+i) = 0.0f;
    for(int i = 0; i < matrix_size; i++)
        for(int j = 0; j < matrix_size; j++)
            *(drifferent_array+i) = *(drifferent_array+i)+(partial_array[i][j]*function_array[j]);
            
    return drifferent_array;
}