#include <stdio.h>
#include <unistd.h>
int main(){
    pid_t pid;
    printf("before fork\n");
    pid = fork();
    if(pid > 0){
        printf("this is father process\n");
    }else if(pid == 0){
        printf("this is son process\n");
    }else if(pid < 0){
        perror("fork");
    }
    return 0;
}

