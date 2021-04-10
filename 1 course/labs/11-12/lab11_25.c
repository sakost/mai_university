#include <stdio.h>

int main(){
    char buf;
    int prev=0, cur = 0;
    int wait_space = 0;
    while(scanf("%c", &buf) > 0){
        if(buf == ' '){
            prev = cur;
            cur = 0;
            wait_space = 0;
            continue;
        }
        if(buf >= 'A' && buf <= 'Z') buf = buf - 'A' + 'a';
        if(wait_space || (!(buf >= 'a' && buf <= 'f') && !(buf >= '0' && buf <= '9') && buf != '\n')){
            wait_space = 1;
            cur = prev;
            continue;
        }
        if(buf == '\n'){
            printf("%x\n", prev);
            prev=0;
            cur=0;
        } else if(buf >= '0' && buf <= '9'){
            cur = cur*16+buf-'0';
        } else if(buf >= 'a' && buf <= 'f'){
            cur = cur * 16 + buf - 'a' + 10;
        }
    }
} 
