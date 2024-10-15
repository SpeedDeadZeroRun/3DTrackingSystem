#include<stdlib.h>
#include<stdio.h>

int multiply_From_To(int from, int to){
    int value_low = to;
    int value_high = from;
    if(from <= to){
        value_low = from;
        value_high = to;
    }
    int return_value = value_low;

    for(int i = value_low+1; i <= value_high; i++)
        return_value = return_value*i;

    return return_value;
}

int nC3(int n){
    if(n < 3) return 0;
    else if(n == 3) return 1;
    int n_minus_3 = n - 3;
    int return_value;
    if(n_minus_3 > 3)
        return_value = multiply_From_To(n, n_minus_3+1)/6;
    else
        return_value = multiply_From_To(n, 4)/multiply_From_To(n_minus_3, 1);
    return return_value;
}