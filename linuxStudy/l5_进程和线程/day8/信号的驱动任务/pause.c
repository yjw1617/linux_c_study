#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle(int sig){
    printf("I get sig = %d\n", sig);
}

void mytask(){
    
    printf("My task start\n");
    sleep(3);
    printf("My task end\n");
}
int main(){
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = handle;  
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGHUP, &act, NULL);

    sigset_t set, set2;
    sigemptyset(&set);
    sigaddset(&set, SIGHUP);
    sigaddset(&set, SIGINT);

    sigemptyset(&set2);
    pause();
    while(1){
        // pause();
        sigprocmask(SIG_BLOCK, &set, NULL);
        mytask();
        // sigprocmask(SIG_UNBLOCK, &set, NULL);
        sigsuspend(&set2);
    }
    


    // sigset_t set;
    // sigemptyset(&set);
    // sigaddset(&set, SIGINT);//屏蔽

    // sigprocmask(SIG_BLOCK, &set, NULL);//将set中的信号添加到屏蔽信号集合中
    // sleep(5);
    // sigprocmask(SIG_UNBLOCK, &set, NULL);//将set中的信号从屏蔽信号集合中删除
    // while(1){
    //     sleep(1);
    // }
}