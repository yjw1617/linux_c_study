#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define POOL_NUM 10
typedef struct Task{
    void *(*func)(void *arg);
    void *arg;
    struct Task *next;
}Task;

typedef struct ThreadPool{
    pthread_mutex_t taskLock;
    pthread_cond_t newTask;

    pthread_t tid[POOL_NUM];
    Task *queue_head;
    int busywork;

}ThreadPool;

ThreadPool *pool;

void *workThread(void *arg){
    while(1){
        pthread_mutex_lock(&pool->taskLock);
        pthread_cond_wait(&pool->newTask,&pool->taskLock);

        Task *ptask = pool->queue_head;
        pool->queue_head = pool->queue_head->next;

        pthread_mutex_unlock(&pool->taskLock);

        ptask->func(ptask->arg);
        pool->busywork--;


    }


}

void *realwork(void *arg){
    printf("Finish work %d\n",(int)arg);

}

void pool_add_task(int arg){
    Task *newTask;
    
    pthread_mutex_lock(&pool->taskLock);
    while(pool->busywork>=POOL_NUM){
        pthread_mutex_unlock(&pool->taskLock);
        usleep(10000);
        pthread_mutex_lock(&pool->taskLock);
    }
    pthread_mutex_unlock(&pool->taskLock);
    

    newTask = malloc(sizeof(Task));
    newTask->func =  realwork;
    newTask->arg = arg;
    

    pthread_mutex_lock(&pool->taskLock);
    Task *member = pool->queue_head;
    if(member==NULL){
        pool->queue_head = newTask;
    }else{
       while(member->next!=NULL){
            member=member->next;
       }
       member->next = newTask;

    }
    pool->busywork++;
    pthread_cond_signal(&pool->newTask);

    pthread_mutex_unlock(&pool->taskLock);


}


void pool_init(){
    pool = malloc(sizeof(ThreadPool));
    pthread_mutex_init(&pool->taskLock,NULL);
    pthread_cond_init(&pool->newTask,NULL);
    pool->queue_head = NULL;
    pool->busywork=0;

    for(int i=0;i<POOL_NUM;i++){
        pthread_create(&pool->tid[i],NULL,workThread,NULL);
    }
}

void pool_destory(){
    Task *head;
    while(pool->queue_head!=NULL){
        head = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        free(head);
    }

    pthread_mutex_destroy(&pool->taskLock);
    pthread_cond_destroy(&pool->newTask);
    free(pool);

}
int main(){
   pool_init();
   sleep(1);
   for(int i=1;i<=200;i++){
       pool_add_task(i);

   }

   sleep(5);
   pool_destory();

}
