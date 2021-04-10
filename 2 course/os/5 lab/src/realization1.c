//
// Created by sakost on 27.11.2020.
//

#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "realization.h"


float SinIntegral(float A, float B, float e){
    float step = e;
    int count = (int)((B - A) / step);

    float ans = 0;
    for (int i = 0; i <= count; i++) {
        ans += step * sinf(A + (float)(i - 1) * step);
    }
    return ans;
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointers"
char* translation(long x){
    char *res = calloc(1, sizeof(char));
    res[0] = '\0';
    if(x == 0){
        char *tmp = calloc(strlen(res) + 2, sizeof(char));
        strcpy(tmp+1, res);
        free(res);
        res = tmp;
        res[0] = '0';
    }
    while(x > 0){
        char ch = (x & 1u) + '0';
        x >>= 1u;
        char *tmp = calloc(strlen(res) + 2, sizeof(char));
        strcpy(tmp+1, res);
        free(res);
        res = tmp;
        res[0] = ch;
    }
    return res;
}
#pragma clang diagnostic pop


