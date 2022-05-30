#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void* testThread(int* param){
    
    printf("this is a thread test\n");
    pthread_exit("thread return");
    
}

int main(){
    void* p1;
    int* p2;
    p2 = p1;//gcc下面不报错  vc下面好像报错
    pthread_t tid;
    int ret;
    void* retv;
    ret = pthread_create(&tid, NULL, (void*)testThread, NULL);
    pthread_join(tid, &retv);//是一个阻塞函数  ,不合适个用在多个线程中 
    printf("pthread return data is %s\n", (char*)retv);
}