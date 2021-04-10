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

    int i = 0, j = 0;
    int flag = 1;
    for(int x = 0; x < 2*k+1; x++){
        for(;i>=0 && j >= 0;i += flag, j -= flag) {
            if(i < k && j < k)
                printf("%d ", byCoord(new_matrix, i, j, k));
        }
        i -= flag;
        j += flag;

        if(j == 0){
            i++;
        } else{
            j++;
        }
        flag = -flag;
    }

    putchar('\n');
} 
