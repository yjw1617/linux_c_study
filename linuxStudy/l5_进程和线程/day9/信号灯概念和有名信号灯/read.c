#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <signal.h>
void delsemfile(int sig){
    sem_unlink("mysem_r");
    exit(0);
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

    //共享内存映射
    char* shmaddr;
    shmaddr = shmat(shmid, NULL, 0);
    if(shmaddr == NULL){
        perror("shmat");
        return -1;
    }

    //信号量
    sem_t *sem_r,*sem_w;

    //打开读信号量
    sem_r = sem_open("mysem_r", O_RDWR | O_CREAT, 0666, 0);//初始化信号量为0
    sem_w = sem_open("mysem_w", O_RDWR | O_CREAT, 0666, 1);

    while(1){
        sem_wait(sem_r);
        // printf(">");
        // fgets(shmaddr, 500, stdin);
        printf("%s", shmaddr);
        sem_post(sem_w);
    }
}