#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

struct iPoint{
    int x, y;
};

int sign(int a){
    return 2*(a >= 0) - 1;
}

int max(int a, int b) {
    if(a > b) return a;
    return b;
}

int min(int a, int b) {
    if(a < b) return a;
    return b;
}

int mod(int a, int b){
    return a - (a / b) * b;
}

const int x1_=-10, y1_=0, x2_=0, y2_=10, x3_ = -10, y3_ = 20;
const struct iPoint v1 = {x1_, y1_}, v2 = {x2_, y2_}, v3 = {x3_, y3_};

int signOfPoint (struct iPoint p1, struct iPoint p2, struct iPoint p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool isPointInArea (int x, int y)
{
    struct iPoint pt = {x, y};
    int d1, d2, d3;
    bool has_neg, has_pos;

    d1 = signOfPoint(pt, v1, v2);
    d2 = signOfPoint(pt, v2, v3);
    d3 = signOfPoint(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}



int main() {
    const int i0 = 8, j0 = 15, l0 = 10;
    int i=i0, j=j0, l=l0, k=0;
    while(!isPointInArea(i, j)){
        if(k==50)break;
        printf("(i,j) = (%d,%d)\n", i, j);
        printf("l = %d\n", l);
        printf("k = %d\n", k);
        int i_prev = i, j_prev = j, l_prev = l;
        i = mod((mod((i_prev + j_prev), (abs(min(j_prev-l_prev, l_prev - k)) + 1)) - k), 20) + 10;
        j = max((i_prev + j_prev)/(2 + sign(j_prev * l_prev - i_prev * k)), (j_prev + l_prev)/(2 + sign(i_prev * j_prev - l_prev*k))) - 10;
        l = mod(max(i_prev, j_prev) * min(i_prev, l_prev), 30);
        k++;
    }
    if(k==50&&!isPointInArea(i, j))
        printf("The point didn't reach the area in 50 steps\n");
    else
        printf("Point reached the area less or equal then 50 steps(%d)\n", k);
    printf("The coordinates are (%d, %d)\n", i, j);
    printf("The dynamic parameter of running is %d\n", l);
}

