#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define POOL_NUM 10
typedef struct Task{
    void*(*func)(void* arg);
    void* arg;
    struct Task* next;
}Task;

typedef struct ThreadPool{
    pthread_mutex_t taskLock;
    pthread_cond_t newTask;

    pthread_t tid[POOL_NUM];
    Task* queue_head;
    int busywork;//忙碌的工作
}ThreadPool;

ThreadPool* pool;

//将线程池中的头结点取出来执行
void* workThread(void* arg){
    pthread_detach(pthread_self());
    while(1){
        pthread_mutex_lock(&pool->taskLock);
        pthread_cond_wait(&pool->newTask, &pool->taskLock);//阻塞等待信号到来
        //将头节点取出来
        Task* ptask = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        pthread_mutex_unlock(&pool->taskLock);
        ptask->func(ptask->arg);
        pool->busywork--;
    }
}

//真正要执行的工作
void* realwork(void* arg){
    printf("Finish work %d\n", (int)arg);
}

//给线程池添加新的Task
void pool_add_task(int arg){
    Task* newTask;
    pthread_mutex_lock(&pool->taskLock);
    while(pool->busywork >= POOL_NUM){
        pthread_mutex_unlock(&pool->taskLock);
        usleep(80000);//让出时间给其他任务运行
        pthread_mutex_lock(&pool->taskLock);
    }
    pthread_mutex_unlock(&pool->taskLock);

    //添加新任务
    newTask = malloc(sizeof(Task));
    newTask->func = realwork;
    newTask->arg = arg;
    // printf("newTask->arg = %d\n", newTask->arg);
    pthread_mutex_lock(&pool->taskLock);
    Task* member = pool->queue_head;
    if(member == NULL){
        pool->queue_head = newTask;
    }else{
        while(member->next != NULL){
            member = member->next;
        }
        member->next = newTask;
    }
    pool->busywork++;
    // printf("pool->busywork = %d\n", pool->busywork);
    pthread_cond_signal(&pool->newTask);//通知线程池中的线程有任务要做了

    pthread_mutex_unlock(&pool->taskLock);
}


//初始化线程池   (装有10个线程)
void pool_init(){
    pool = malloc(sizeof(ThreadPool));
    pthread_mutex_init(&pool->taskLock, NULL);
    pthread_cond_init(&pool->newTask, NULL);
    pool->queue_head = NULL;
    pool->busywork = 0;

    for(int i = 0; i < POOL_NUM; i++){
        pthread_create(&pool->tid[i], NULL, workThread, NULL);
    }


}

//线程的销毁
void pool_destory(){
    Task* head;
    while(pool->queue_head != NULL){
        head = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        free(head);
    }

    pthread_mutex_destroy(&pool->taskLock);
    pthread_cond_destroy(&pool->newTask);
    free(pool);
}
int main(){
    //线程池的初始化
    pool_init();
    sleep(1);
    for(int i = 0; i < 20; i++){
        pool_add_task(i);
    }

    sleep(5);
    pool_destory();
    return 0;
}