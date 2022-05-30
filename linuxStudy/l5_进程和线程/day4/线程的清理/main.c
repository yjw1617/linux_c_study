#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* cleanup(void* arg){
    printf("cleanup , arg = %s\n", (char*)arg);
}

int* testThread(int* param){
    printf("this is child process\n");
    pthread_cleanup_push(cleanup, (void*)"abcd");//这个是堆栈的机制 可以和pop一起写多对
    
    while(1){
        sleep(1);
    }
    pthread_exit(NULL);
    pthread_cleanup_pop(0);//0不执行cleanup 1会执行回调函数
}

int main(){
    pthread_t tid;
    int ret;
    pthread_create(&tid, NULL, (void*)testThread, NULL);
    sleep(1);
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    while(1){
        sleep(1);
    }
}

//可以使用top -p 加pid号来时时查看占用大小