#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(){
    int pfd[2];
    int ret;
    pid_t pid[2];
    char buf[50];
    ret = pipe(pfd);
    int i = 0;
    if(ret < 0){
        perror("pipe");
        return -1;
    }
    for(i = 0; i < 2; i++){
        pid[i] = fork();
        if(pid[i] < 0){
            perror("fork");
            return 0;
        }else if(pid[i] > 0){

        }else{
            break;
        }
    }
    if(i == 0){//子进程1
        close(pfd[0]);
        while(1){
            strcpy(buf, "process 1 write hahahhahahah");
            write(pfd[1], buf, strlen(buf));
            sleep(5);
        }
    }
    if(i == 1){//子进程2
        close(pfd[0]);
        while(1){
            strcpy(buf, "process 2 write hahahhahahah");
            write(pfd[1], buf, strlen(buf));
            sleep(1);
        }
    }
    if(i == 2){//父进程
        close(pfd[1]);
        while(1){
            ret = read(pfd[0], buf, 100);
            if(ret > 0){
                printf("father read pipe = %s\n", buf);
            }
        }   
        int status;
        waitpid(pid[0], &status, 0);
        waitpid(pid[1], &status, 0);
    }
    return 0;
}