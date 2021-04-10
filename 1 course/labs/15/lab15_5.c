#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

const int maxn = 8;

int byCoord(const int *matrix, int x, int y, int n){
    return matrix[x + y * n];
}

void iarrcpy(int *dest, const int* src, int n){
    for (int i = 0; i < n; ++i) {
        dest[i] = src[i];
    }
}

void print_matrix(int *matrix, int n){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", byCoord(matrix, j, i, n));
        }
        printf("\n");
    }
}

int main(){
    freopen("in.txt", "r", stdin);

    int *startMatrix = calloc(maxn, sizeof(int));
    memset(startMatrix, 0, sizeof(startMatrix));
    char buf;
    int n = 0;
    while(scanf("%d%c", startMatrix + n++, &buf) > 0){
        if(buf == '\n')
            break;
    }
    // n now is a size of matrix
    int *matrix = calloc(n*n, sizeof(n));
    memset(matrix, 0, sizeof(matrix));

    iarrcpy(matrix, startMatrix, n);
    free(startMatrix);

    for (int i = n; i < n*n; ++i) {
        scanf("%d", matrix+i);
    }

    int mmax = matrix[0];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(byCoord(matrix, j, i, n) > mmax){
                mmax = byCoord(matrix, j, i, n);
            }
        }
    }

    bool *has_max = calloc(n, sizeof(bool));
    memset(has_max, false, sizeof(has_max));

    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            if(byCoord(matrix, col, row, n) == mmax){
                has_max[row] = true;
                break;
            }
        }
    }

    int *sum = calloc(n, sizeof(int));
    memset(sum, 0, sizeof(sum));

    for (int row = 0; row < n; ++row) {
        if(!has_max[row]) continue;
        for (int col = 0; col < n; ++col) {
            sum[col] += byCoord(matrix, col, row, n);
        }
    }

    for (int row = 0; row < n; ++row) {
        if(has_max[row]){
            for (int col = 0; col < n; ++col) {
                matrix[col + row*n] = sum[col];
            }
            break;
        }
    }

    print_matrix(matrix, n);

    free(matrix);
}
