#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
    char *filename;
    size_t len;

    printf("Enter a filename with tests: ");
    if(getline(&filename, &len, stdin) == -1){
        perror("getline");
    }

    filename[strlen(filename) - 1] = '\0';

    int fds[2];
    if(pipe(fds) != 0){
        perror("pipe");
    }

    int rfd = fds[0];
    int wfd = fds[1];

//    if(access(filename, R_OK) == -1){
//        fprintf(stderr, "File not exists\n");
//        exit(EXIT_FAILURE);
//    }
    int file = open(filename, O_RDONLY);
    if (file == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if(pid == 0){
        if (dup2(wfd, fileno(stdout)) == -1) {
            exit(1);
        }
        close(wfd);

        if (dup2(file, fileno(stdin)) == -1) {
            exit(1);
        }
        execl("2_lab_child", "2_lab_child", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
    else if(pid == -1){
        perror("fork");
    }

    close(fds[1]);
    close(file);

    char c;
    while(read(rfd, &c, sizeof(char)) > 0){
        putchar(c);
    }

    int status;
    if(wait(&status) == -1){
        perror("wait");
    }
    if(!WIFEXITED(status) || (WIFEXITED(status) && (WEXITSTATUS(status)) != 0)){
        fprintf(stderr, "Some error occurred in child process\n");
        return 1;
    }
    return 0;
}
