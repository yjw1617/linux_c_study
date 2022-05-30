#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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
        alarm(1);
    }
}

int main(){
    
    act.sa_handler = sigHandle;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, &o_act);
    sigaction(SIGALRM, &act, &o_act);
    alarm(1);
    
    // oldact = signal(SIGINT, sigHandle);
    // sigaction();
    // signal(SIGQUIT, sigHandle);
    while(1){
        sleep(1);
    }
}