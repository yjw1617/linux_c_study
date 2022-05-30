#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
void sigHandle(int sig){
    printf("get sig = %d\n", sig);
    wait(NULL);//通过信号来回收子进程
}
int main(){
    pid_t pid;
    struct sigaction act;
    act.sa_handler = sigHandle;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);//屏蔽位清0  接收所有信号

    pid = fork();
    if(pid > 0){
        sigaction(SIGCHLD, &act, NULL);
        while(1){
            printf("this is father process\n");
            sleep(1);
        }
    }else if(pid == 0){
        sleep(5);
        exit(0);//子进程状态改变发送给父进程SIGCHLD信号
    }
    return 0;
}