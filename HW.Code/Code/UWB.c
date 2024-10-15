#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"FunctionLibrary/UWB_Math.h"
#include"FunctionLibrary/UWB_nC3.h"
#include"FunctionLibrary/UWB_Matrix.h"
#include"FunctionLibrary/UWB_Newton.h"

#define anchor_count 4
#define matrix_size 3

int main(){
    struct nC3* structure = nC3_Creator(anchor_count);

    // {X, Y, Z} units:milimeter
    float anchor_location[4][matrix_size] = {
        {519.618, 876.596, -51.963}, // Anchor A
        {19.6184, 10.5662, -51.963}, // Anchor B
        {1019.62, 10.5648, -51.963}, // Anchor C
        {519.618, 299.242, 764.531}  // Anchor D
    };
    // Tag locate at 692.93, 282.299, 834.737
    float tag_distance[4] = {1081.42, 1146.05, 983.262, 187.758};
    float guess[matrix_size] = {0.0, 0.0, 0.0}; // initial guess

    float current_anchor_location[matrix_size][matrix_size] = {0.0};
    float current_distance[matrix_size] = {0.0};
    float tags_location[4][matrix_size] = {0.0};
    for(int i = 0; i < structure->nC3_size; i++){
        // newton_3b3_3 loader
        for(int j = 0; j < matrix_size; j++){
            int choose_row = *(structure->nC3_array+(matrix_size*i)+j);
            memcpy(current_anchor_location[j], anchor_location[choose_row], sizeof(float)*matrix_size);
            current_distance[j] = tag_distance[choose_row];
        }

        // current newton method round
        for(int j = 0; j < 5; j++){
            float *guess_location = newton_3b3_3(guess, current_anchor_location, current_distance);
            memcpy(guess, guess_location, sizeof(float)*matrix_size);
            free(guess_location);
        }

        // put result to array
        memcpy(tags_location[i], guess, sizeof(float)*matrix_size);
    }

    // print out
    char anchor_label[anchor_count] = {'A', 'B', 'C', 'D'} ;
    for(int i = 0; i < structure->nC3_size; i++){
        for(int j = 0; j < matrix_size; j++)
            printf("%c ", anchor_label[*(structure->nC3_array+(matrix_size*i)+j)]);
        for(int j = 0; j < matrix_size; j++)
            printf("%.3f ", tags_location[i][j]);
        printf("\n");
    }
    printf("true location: 692.930 282.299 834.737\n");

    nC3_Destroyer(structure);
    
    return 0;
}