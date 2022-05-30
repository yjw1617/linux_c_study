#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
    pid_t pid;
    pid_t rpid;
    pid = fork();
    int status;
    if(pid == 0){
        sleep(1);
        printf("son will exit\n");
        exit(9);
    }else if(pid > 0){
        printf("this is father\n");
        // rpid = wait(&status);//相当于waitpid(-1, &status, 0)
        waitpid(pid, &status, 0);
        printf("Get chile status = %x\n", WEXITSTATUS(status));
    }else{
        perror("fork");
        return 0;
    }
}