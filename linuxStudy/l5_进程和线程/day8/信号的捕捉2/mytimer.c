#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
struct sigaction act, o_act;
void sigHandle(int sig){
    if(sig == SIGINT){
        printf("I cath the SIGINT\n");
        sigaction(SIGINT, &o_act, NULL); 
        // signal(SIGINT, oldact); 
    }
    if(sig == SIGQUIT){
        printf("I cath the SIGQUIT\n"); 
    }
    if(sig == SIGALRM){
        printf("I cath the SIGALRM\n");  
    }
}

int main(){
    
    act.sa_handler = sigHandle;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, &o_act);
    sigaction(SIGALRM, &act, &o_act);
    struct itimerval timevalue;
    timevalue.it_interval.tv_sec = 1;//周期一秒
    timevalue.it_interval.tv_usec = 0;
    timevalue.it_value.tv_sec = 5;//5s启动定时器
    timevalue.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &timevalue, NULL);
    
    // oldact = signal(SIGINT, sigHandle);
    // sigaction();
    // signal(SIGQUIT, sigHandle);
    while(1){
        // sleep(1);
    }
}