#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* char_append(char* arr, char el){
    char *new_arr = calloc(strlen(arr) + 2, sizeof(char));
    strcpy(new_arr, arr);
    new_arr[strlen(arr)] = el;
    new_arr[strlen(arr)+1] = '\0';
    free(arr);
    return new_arr;
}

int main(){
    freopen("in.txt", "r", stdin);

    char buf;
    char* str = calloc(1, sizeof(char));
    str[0] = '\0';
    while(scanf("%c", &buf) > 0){
        str = char_append(str, buf);
    }

    for (size_t i = 0;  i < strlen(str); i+=2) {
        printf("%c", str[i]);
    }
} 
