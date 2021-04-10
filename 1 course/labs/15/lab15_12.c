 усло 
#include <stdio.h>
#include <malloc.h>

const int maxn = 8;
const int group_num = 8;

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

void print_matrix(int *matrix, int n){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", byCoord(matrix, j, i, n));
        }
        putchar('\n');
    }
}

void matrix_shift_counter_clockwise(int *matrix, int n){
    int dir[4] = {1, 0, -1, 0};
    int i = 0, j = -1;
    int l = n;
    int p = 0;
    int el = matrix[0];
    while (l != 0){
        for (int k = 0; k < l; ++k) {
            j += dir[p % 4];
            i += dir[(p+3) % 4];
            int t = matrix[i + j * n];
            matrix[i + j * n] = el;
            el = t;
        }
        ++p;
        l -= p % 2;
    }
    matrix[0] = el;
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

    // var 12
    for (int i = 0; i < group_num; ++i) {
        matrix_shift_counter_clockwise(new_matrix, k);
    }

    print_matrix(new_matrix, k);
    free(new_matrix);
}
