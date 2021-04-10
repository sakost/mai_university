 
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int from_hex(char ch){
    return ch - '0';
}

int main(){
    freopen("in.txt", "r", stdin);
    char buf;
    bool pass = false;
    unsigned n = 0;
    while(scanf("%c", &buf) > 0){
        if(!isdigit(buf)){
            if(buf == ' ' || buf == '\n' || buf == '\t' || buf == ','){
                if(!pass){
                    printf("%u ", ~n);
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
