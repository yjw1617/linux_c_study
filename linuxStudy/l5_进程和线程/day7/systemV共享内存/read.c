#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
int main(){
    key_t key;
    int shmid;
    char* buf;
    //生成key
    key = ftok("test", 100);
    if(key < 0){
        perror("ftok");
        return -1;
    }
    printf("key = %d\n", key);

    //创建共享内存
    shmid = shmget(key, 512, 0666);
    if(shmid < 0){
        perror("shmget");
        return -1;
    }    

    printf("shmid = %d\n", shmid);

    //映射共享内存
    buf = (char*)shmat(shmid, NULL, 0);//写0可读可写
    if(buf == NULL){
        perror("shmat");
        return -1;
    }
    // strcpy(buf, "hello world");
    printf("data is %s\n", buf);

    shmdt(buf);
    shmctl(shmid, IPC_RMID, NULL);
}