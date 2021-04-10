#include <stdio.h>
#include <time.h>

int main(){
        time_t timer;
        struct tm* tm_info;
        char buf[26];
        time(&timer);
        tm_info = localtime(&timer);
        strftime(buf, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        puts(buf);
        return 0;
}
