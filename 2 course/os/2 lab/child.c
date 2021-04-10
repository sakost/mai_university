//
// Created by sakost on 23.09.2020.
//
#include <stdio.h>
#include <unistd.h>

int main(){
    double a;
    char c;
    double res = 0;

    while(scanf("%lf%c", &a, &c) != EOF) {
        res += a;
        if(c == '\n') {
            printf("%lf\n", res);
            fflush(stdout);
            res = 0.;
            continue;
        }
    }

    printf("%lf\n", res);
    close(STDOUT_FILENO);
    return 0;
}

