#include "realization.h"

#include <stdio.h>

int main() {
    int cmd = 0;

    while (scanf("%d", &cmd) != EOF) {

        float a, b, c;
        if (scanf("%f %f %f", &a, &b, &c) == EOF) {
            break;
        }

        if (cmd == 1) {
            puts("Calculate sin(x)");
            printf("%f\n", SinIntegral(a, b, c));
        }
    }
}
