#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int* testThread(int* param){
    pthread_detach(pthread_self());
    // printf("this is %d thread\n", param);
    while(1){
        sleep(25);
        printf("dsad\n");
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t tid;
    int ret;
    pthread_create(&tid, NULL, (void*)testThread, NULL);
    pthread_cancel(tid);//杀死线程
    while(1){
        sleep(1);
    }
}

//可以使用top -p 加pid号来时时查看占用大小