#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void* testThread(void* param){
    pthread_detach(pthread_self());
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);//时能不能被取消
    {
        sleep(5);
        pthread_testcancel();//创建一个取消点
    }
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);//时能可以被取消
    while(1){
        sleep(1);
        printf("run\n");
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t tid;
    int ret;
    pthread_create(&tid, NULL, (void*)testThread, NULL);
    sleep(8);
    pthread_cancel(tid);//杀死线程
    while(1){
        sleep(1);
    }
}

//可以使用top -p 加pid号来时时查看占用大小