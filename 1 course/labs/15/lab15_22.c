 
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
    int mmin = byCoord(new_matrix, 0, 0, k);
    int mi = 0, mj = 0;
    // var 22
    for (int j = 0; j < k; ++j) {
        for(int i = 0;i < k; ++i){
            if(byCoord(new_matrix, i, j, k) < mmin){
                mmin = byCoord(new_matrix, i, j, k);
		mi = i;
		mj = j;
            }
        }
    }

    

    for (int i = 0; i < k/2; ++i) {
        swap(new_matrix + i + mj*k, new_matrix + k - i - 1 + mj*k);
    }
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            printf("%d ", byCoord(new_matrix, j, i, k));
        }
        putchar('\n');
    }
    free(new_matrix);
}
