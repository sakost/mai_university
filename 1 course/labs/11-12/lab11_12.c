 
#include <stdio.h>
#include <stdbool.h>

int sign(int n){
    if (n >= 0) return 1;
    return -1;
}

int main(){
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    char buf;

    int res = 0;
    bool wait_sp = false;
    bool minus = false;

    while(scanf("%c", &buf) > 0){
        if(buf == ' ' || buf == '\n'){
            if(minus){
                printf("-");
            }
            if(!wait_sp)
                printf("0%d\n", res);
            wait_sp = 0;
            res = 0;
            minus = false;
            continue;
        }
        if(buf == '-' && minus == false){
            minus = true;
            continue;
        } else if(buf == '-'){
            minus = false;
            wait_sp = 1;
            continue;
        }

        if(wait_sp || !(buf >= '0' && buf <= '9')){
            res = 0;
            wait_sp = 1;
            continue;
        }


        if(buf >= '0' && buf <= '9'){
            res = 10*res + sign(res) * (buf - '0');
        } else{
            res = 0;
            wait_sp = 1;
            continue;
        }
    }
}
