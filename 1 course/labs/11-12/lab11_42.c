#include <stdio.h>
#include <stdbool.h>


bool is_allowed_symbol(char ch){
    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z');
}

int from_hex(char ch){
    if(ch <= '9' && ch >= '0'){
        return ch - '0';
    }
    return ch - 'a' + 10;
}


char cto_hex(int n){
    if(n < 10){
        return n+'0';
    }
    return 'a' + n - 10;
}




int main(){
    freopen("in.txt", "r", stdin);
    char buf;
    int f_num = 0, s_num = 0;
    bool first = true;
    int cnt = 0;
    while(scanf("%c", &buf) > 0){
        if(!is_allowed_symbol(buf)) {
            int rev_res, res = 0;
            if(cnt % 2 == 0){
                rev_res = f_num;
            } else rev_res = s_num;
            f_num = s_num = cnt = 0;
            first = true;

            while(rev_res > 0){
                int t = rev_res % 16;
                res = 16 * res + t;
                rev_res /= 16;
            }
            while(res > 0){
                int k = res % 16;
                res /= 16;
                printf("%c", cto_hex(k));
            }
            printf("\n");
        }else {
            if(first) {
                f_num = f_num * 16 + from_hex(buf);
                first = false;
            } else{
                s_num = s_num * 16 + from_hex(buf);
                first = true;
            }
            cnt++;
        }
    }
} 
