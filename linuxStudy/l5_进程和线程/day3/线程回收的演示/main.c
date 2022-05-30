#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int* testThread(int* param){
    pthread_detach(pthread_self());
    printf("this is %d thread\n", param);
    sleep(25);
    pthread_exit(NULL);
}

int main(){
    pthread_t tid[5];
    int ret;

    for(int i = 0; i < 100; i++){
        ret = pthread_create(&tid[i], NULL, (void*)testThread, (int*)i);
        // pthread_detach(tid[i]);//第一种方法
    }
    while(1){
        sleep(1);
    }
}

//可以使用top -p 加pid号来时时查看占用大小