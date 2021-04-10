#include <stdio.h>
#include <stdbool.h>

bool isCharInWord(char ch){
    return (ch != ' ' && ch != ',' && ch != '\t' && ch != EOF && ch != '\n');
}


int main(){
    freopen("in.txt", "r", stdin);

    char buf;
    int ans = 0, cnt = 0;

    while(scanf("%c", &buf) > 0){
        if(isCharInWord(buf)){
            cnt++;
        } else{
            if(cnt >= 3) ans++;
            cnt = 0;
            if(buf == '\n'){
                printf("%d\n", ans);
                ans = 0;
            }
        }
    }
    if(buf != '\n')
        printf("%d", ans);
}
