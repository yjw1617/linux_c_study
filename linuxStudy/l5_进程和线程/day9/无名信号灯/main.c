#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <signal.h>

#include <pthread.h>

//信号量
sem_t sem_r, sem_w;


//共享内存映射
char* shmaddr;
void delsemfile(int sig){
    sem_destroy(&sem_r);
    sem_destroy(&sem_w);
    exit(0);
}

void* readmemTask(void*param){
    pthread_detach(pthread_self());
    while(1){
        sem_wait(&sem_r);
        printf("%s", shmaddr);
        sem_post(&sem_w);
    }
}
int main(){
    key_t key;
    key = ftok(".", 100);
    if(key < 0){
        perror("ftok");
        return -1;
    }
    //捕捉信号
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = delsemfile;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);
    //申请共享内存
    int shmid;
    shmid = shmget(key ,500, IPC_CREAT | 0666);
    if(shmid < 0){
        perror("shmget");   
        return -1;
    }

    
    shmaddr = shmat(shmid, NULL, 0);
    if(shmaddr == NULL){
        perror("shmat");
        return -1;
    }

    

    //打开读信号量
    // sem_r = sem_open("mysem_r", O_RDWR | O_CREAT, 0666, 0);//初始化信号量为0
    // sem_w = sem_open("mysem_w", O_RDWR | O_CREAT, 0666, 1);
    sem_init(&sem_r, 0, 0);//第一次不可以读
    sem_init(&sem_w, 0, 1);//第一次可以写

    //创建线程
    pthread_t tid;
    pthread_create(&tid, NULL, readmemTask, NULL);
    while(1){
        sem_wait(&sem_w);
        printf(">");
        fgets(shmaddr, 500, stdin);
        sem_post(&sem_r);
    }
}