#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int* testThread(int* param){
    printf("this is %d thread\n", param);
    while(1){
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t tid[5];
    int ret;

    for(int i = 0; i < 5; i++){
        ret = pthread_create(&tid[i], NULL, (void*)testThread, (int*)i);
    }
    while(1){
        
    }
}