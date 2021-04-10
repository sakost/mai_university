 
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
    freopen("in.txt", "r", stdin);

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

    int l = 1;
    int p = 0;
    int x = k / 2 - (k - 1) % 2, y = k / 2 - (k - 1) % 2;
    while(l != k+1){
        for(int s = 0; s != l; ++s){
            if(x < k && y < k && x >= 0 && y >= 0)
                printf("%d ", byCoord(new_matrix, x, y, k));
            y += dir[(p + 3) % 4];
            x += dir[(p) % 4];
        }
        l += p%2;
        ++p;
    }
//    printf("%d ", byCoord(new_matrix, x, y, k));
    putchar('\n');
}
