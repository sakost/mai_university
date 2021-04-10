 
#include <stdio.h>
#include <malloc.h>

const int maxn = 8;

int byCoord(int *matrix, int x, int y, int n){
    return matrix[x + y*n];
}

void copy(int* dest, const int* src, int n){
    for (int i = 0; i < n; ++i) {
        dest[i] = src[i];
    }
}

void swap(int *a, int *b){
    int c = *a;
    *a = *b;
    *b = c;
}

int main(){
    freopen("in.txt", "r", stdin);

    char buf;
    int *matrix = calloc(maxn, sizeof(int));
    int k = 0;
    while(scanf("%d%c", matrix + k++, &buf) > 0){
        if(buf == '\n'){
            break;
        }
    }
    int *new_matrix = calloc(k*k, sizeof(int));
    copy(new_matrix, matrix, k);
    free(matrix);
    for (int i = k; i < k*k; ++i) {
        scanf("%d", new_matrix + i);
    }

    // var 4
    int *product = calloc(k-1, sizeof(int));
    for (int j = 0; j < k; ++j) {
        product[j] = 1;
        for (int i = 0; i < k; ++i) {
            product[j] *= byCoord(new_matrix, j, i, k);
        }
    }

    int imax=0, elmax=product[0], imin=0, elmin=product[0];
    for (int i = 0; i < k; ++i) {
        int cur = product[i];
        if(cur > elmax){
            elmax = cur;
            imax = i;
        }
        if(cur < elmin){
            elmin = cur;
            imin = i;
        }
    }

    for (int i = 0; i < k; ++i) {
        swap(new_matrix + i + imin*k, new_matrix + i + imax * k);
    }
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            printf("%d ", byCoord(new_matrix, j, i, k));
        }
        putchar('\n');
    }
    free(new_matrix);
}
