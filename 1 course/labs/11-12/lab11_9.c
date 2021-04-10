#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char to_hex(int n){
    return n + '0';
}

int from_hex(char ch){
    return ch - '0';
}

bool is_good(int n){
    while(n>0){
        if(n%10 > 2) return true;
        n /= 10;
    }
    return false;
}

int main(){
    freopen("in.txt", "r", stdin);
    char buf;
    bool pass = false;
    int n = 0;
    while(scanf("%c", &buf) > 0){
        if(!isdigit(buf) || (isdigit(buf) && buf > '4')){
            if(buf == ' ' || buf == '\n' || buf == '\t' || buf == ','){
                if(is_good(n) && !pass){
                    printf("%d ", n);
                }
                pass = false;
            } else
                pass = true;
            n = 0;
            continue;
        }
        if(pass)continue;
        n += from_hex(buf);
    }
} 
