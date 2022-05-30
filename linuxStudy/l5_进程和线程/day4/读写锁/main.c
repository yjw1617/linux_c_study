#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
FILE* fp;
pthread_rwlock_t rwlock;


void* read_func(void* arg){
    pthread_detach(pthread_self());
    printf("read process\n");
    char rd_buff[32] = {0};
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        rewind(fp);
        while(fgets(rd_buff, 32, fp) != NULL){
            printf("%d :rd = %s\n", (int)arg, rd_buff);
            sleep(1);
        }
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
}   

void* func1(void* param){
    pthread_detach(pthread_self());
    printf("this is func1 process\n");
    char str[] = "I write func1\n";
    char c = 0;
    int i = 0;
    while(1){
        pthread_rwlock_wrlock(&rwlock);
        fseek(fp, 0, SEEK_END);
        while(i < strlen(str)){
            c = str[i];
            fputc(c, fp);
            usleep(1);
            i++;
        }
        pthread_rwlock_unlock(&rwlock);
        i = 0;
        sleep(5);
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
        pthread_rwlock_wrlock(&rwlock);
        fseek(fp, 0, SEEK_END);
        while(i < strlen(str)){
            c = str[i];
            fputc(c, fp);
            usleep(1);
            i++;
        }
        pthread_rwlock_unlock(&rwlock);
        i = 0;
        sleep(3);

    }
    pthread_exit("func2 exit");
}

int main(){
    pthread_t tid1, tid2, tid3, tid4;
    int ret;

    fp = fopen("1.txt","a+");
    if(fp == NULL){
        perror("fopen");
        return 0;
    }
    pthread_rwlock_init(&rwlock, NULL);
    
    
    

    pthread_create(&tid1, NULL, func1, NULL);
    pthread_create(&tid2, NULL, func2, NULL);
    pthread_create(&tid3, NULL, read_func, 1);
    pthread_create(&tid4, NULL, read_func, 2);
    
    
    
    while(1){
        sleep(1);
    }
}

//可以使用top -p 加pid号来时时查看占用大小