#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int* testThread(int* param){
    while(1){
        printf("param = %d\n", *param);
        printf("self pid = %ld\n", pthread_self());
        sleep(1);
        printf("this is a thread test\n");
        pthread_exit(NULL);
        printf("after test\n");
    }
}

int main(){
    pthread_t tid;
    int ret;
    int a = 10;
    ret = pthread_create(&tid, NULL, (void*)testThread, &a);
    while(1){

    }
}