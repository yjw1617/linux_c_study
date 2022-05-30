#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int* testThread(int* param){
    // pthread_detach(pthread_self());
    printf("this is %d thread\n", param);
    while(1){
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t tid[5];
    int ret;
    pthread_attr_t at;
    pthread_attr_init(&at);
    pthread_attr_setdetachstate(&at, PTHREAD_CREATE_DETACHED);//设置分离属性
    for(int i = 0; i < 5; i++){
        ret = pthread_create(&tid[i], &at, (void*)testThread, (int*)i);
        // pthread_detach(tid[i]);//第一种方法
    }
    while(1){
        
    }
}