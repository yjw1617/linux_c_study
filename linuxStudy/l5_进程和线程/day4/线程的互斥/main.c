#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
FILE* fp;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* func1(void* param){
    pthread_detach(pthread_self());
    printf("this is func1 process\n");
    char str[] = "I write func1\n";
    char c = 0;
    int i = 0;
    while(1){
        pthread_mutex_lock(&mutex);
        while(i < strlen(str)){
            c = str[i];
            fputc(c, fp);
            usleep(1);
            i++;
        }
        pthread_mutex_unlock(&mutex);
        i = 0;
        usleep(1);
    }
    pthread_exit("func1 exit");
}

void* func2(void* param){
    pthread_detach(pthread_self());
    printf("this is func2 process\n");
    char str[] = "you write func2\n";
    char c = 0;
    int i = 0;
    while(1){
        pthread_mutex_lock(&mutex);
        while(i < strlen(str)){
            c = str[i];
            fputc(c, fp);
            usleep(1);
            i++;
        }
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