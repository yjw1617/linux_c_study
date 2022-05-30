#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <pthread.h>
#define SEM_READ 0
#define SEM_WRITE 1

union semun{
    /* data */
    int val;
};

void Poperation(int id, int semindex){
    struct sembuf sbuf;
    sbuf.sem_num = semindex;
    sbuf.sem_op = -1;//消耗资源
    sbuf.sem_flg = 0;
    semop(id, &sbuf, 1);//信号灯个数为1
}

void Voperation(int id, int semindex){
    struct sembuf sbuf;
    sbuf.sem_num = semindex;
    sbuf.sem_op = 1;//释放资源
    sbuf.sem_flg = 0;
    semop(id, &sbuf, 1);//信号灯个数为1
}

int main(){
    //创建key
    key_t key;
    key = ftok(".", 100);
    if(key < 0){
        perror("ftok");
        return -1;
    }

    //创建信号灯
    int semid;

    semid = semget(key, 2, IPC_CREAT | 0666);
    if(semid < 0){
        perror("semget");
        return -1;
    }

    //创建一个共享内存
    int shmid;
    shmid = shmget(key, 500, IPC_CREAT | 0666);

    //映射共享内存地址
    char* shmaddr;
    shmaddr = shmat(shmid, NULL, 0);
    if(shmaddr == NULL){
        perror("shmat");
        return -1;
    }

    union semun mysem;
    mysem.val = 0;
    semctl(semid, SEM_READ, SETVAL, mysem);

    mysem.val = 1;
    semctl(semid, SEM_WRITE, SETVAL, mysem);

    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork");
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
    }else if(pid == 0){
        while(1){
            Poperation(semid, SEM_READ);
            printf("%s\n", shmaddr);
            Voperation(semid, SEM_WRITE);
        }
    }else{
        while(1){
            Poperation(semid, SEM_WRITE);
            printf(">");
            fgets(shmaddr, 32, stdin);
            Voperation(semid, SEM_READ);
        }
    }
    return 0;
}