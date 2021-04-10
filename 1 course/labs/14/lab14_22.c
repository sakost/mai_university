#include <stdio.h>
#include <malloc.h>

int byCoord(int *matrix, int x, int y, int n){
    return matrix[x + y*n];
}

void copy(int* dest, const int* src, int n){
    for (int i = 0; i < n; ++i) {
        dest[i] = src[i];
    }
}

int main(){
    char buf;
    int *matrix = calloc(7, sizeof(int));
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

    int dir[4] = {1, 0, -1, 0};

    int l = k;
    int p = 0;
    int i = 0, j = -1;
    while(l != 0){
        for(int x = 0; x != l; ++x){
            j += dir[p%4];
            i += dir[(p+3)%4];
            printf("%d ", byCoord(new_matrix, i, j, k));
        }
        ++p;
        l -= p%2;
    }
    putchar('\n');
} 
