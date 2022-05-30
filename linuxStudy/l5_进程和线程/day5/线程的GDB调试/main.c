#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* testTask(void* arg){
    char* thread_name = (char*)arg;
    printf("%s  running\n", thread_name);
    printf("aaaaaaaaa\n");
    printf("bbbbbbbbb\n");

    pthread_exit(NULL);
}

int main(){
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, testTask, "thread1");
    pthread_create(&tid2, NULL, testTask, "thread2");
    // while(1){

    // }
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

}