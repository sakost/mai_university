//
// Created by sakost on 27.11.2020.
//

#include "realization.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

float SinIntegral(float A, float B, float e){
    int n = (int)((B - A) / e);

    float h = (B-A)/(float)n;

    float s = sinf(A)+sinf(B);

    for (int i = 1; i < n; i++)
        s += 2*sinf(A+(float)i*h);

    return h*s/2;
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
        char ch = (x % 3) + '0';
        x /= 3;
        char *tmp = calloc(strlen(res) + 2, sizeof(char));
        strcpy(tmp+1, res);
        free(res);
        res = tmp;
        res[0] = ch;
    }
    return res;
}
#pragma clang diagnostic pop
