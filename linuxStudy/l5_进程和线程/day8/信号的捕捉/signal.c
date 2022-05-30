#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
sig_t oldact;
void sigHandle(int sig){
    if(sig == SIGINT){
        printf("I cath the SIGINT\n"); 
        signal(SIGINT, oldact); 
    }
    if(sig == SIGQUIT){
        printf("I cath the SIGQUIT\n"); 
    }
    
}

int main(){
    
    oldact = signal(SIGINT, sigHandle);
    signal(SIGQUIT, sigHandle);
    while(1){
        sleep(1);
    }
}