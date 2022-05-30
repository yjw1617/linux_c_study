#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
//如果两个线程获取锁的顺序相同那么就不会造成死锁
//避免死锁 最好使用一把锁  
//调整好锁的顺序
FILE* fp;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
void* func1(void* arg){
    pthread_detach(pthread_self());
    printf("this is func1 process\n");
    char str[] = "I write func1\n";
    char c = 0;
    int i = 0;
    while(1){
        pthread_mutex_lock(&mutex2);
        printf("%d I got lock 2\n",(int)arg);
        sleep(1);
        pthread_mutex_lock(&mutex);
        printf("%d I got 2 lock\n",(int)arg);
        
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&mutex2);
        i = 0;
        usleep(1);
    }
    pthread_exit("func1 exit");
}

void* func2(void* arg){
    pthread_detach(pthread_self());
    printf("this is func2 process\n");
    char str[] = "you write func2\n";
    char c = 0;
    int i = 0;
    while(1){
        pthread_mutex_lock(&mutex);
        printf("%d I got lock 1\n",(int)arg);
        sleep(1);
        pthread_mutex_lock(&mutex2);
        printf("%d I got 2 lock\n",(int)arg);

        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex);
        i = 0;
        usleep(1);

    }
    pthread_exit("func2 exit");
}

int main(){
    pthread_t tid1, tid2;
    int ret;

    fp = fopen("1.txt","a+");
    if(fp == NULL){
        perror("fopen");
        return 0;
    }
    pthread_create(&tid1, NULL, (void*)func1, NULL);
    pthread_create(&tid2, NULL, (void*)func2, NULL);

    while(1){
        sleep(1);
    }
}

//可以使用top -p 加pid号来时时查看占用大小