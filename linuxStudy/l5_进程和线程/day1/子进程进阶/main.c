#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid;
    int i = 0;
    for(i = 0; i < 5; i++){
        pid = fork();
        if(pid < 0){
            perror("fork");
            return 0;
        }else if(pid == 0){
            printf("child process\n");
            sleep(5);
            break;
        }else{
            printf("father process\n");
            sleep(5);
        }
    }
    return 0;
}