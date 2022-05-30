#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int* testThread(int* param){
    while(1){
        sleep(1);
        printf("this is a thread test\n");
        pthread_exit(NULL);
        printf("after test\n");
    }
}

int main(){
    pthread_t tid;
    int ret;
    ret = pthread_create(&tid, NULL, (void*)testThread, NULL);
    while(1){

    }
}