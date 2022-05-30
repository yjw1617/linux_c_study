#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
//发送消息
    typedef struct{
        long msg_type;
        char buf[128];
    }msgT;
int main(){
    key_t key;
    key = ftok(".", 100);
    if(key < 0){
        perror("ftok");
        return -1;
    }

    int msgid = msgget(key, IPC_CREAT|0666);
    if(msgid < 0){
        perror("msgget");
        return -1;
    }

    //接收的消息
    msgT mymsg = {0};
    //发送消息动作
    int ret = 0;
    int num = 0;
    while(0 < msgrcv(msgid, &mymsg, sizeof(msgT) - sizeof(long), 1, 0) && num < 5){
        // ret = msgrcv(msgid, &mymsg, sizeof(msgT) - sizeof(long), 1, 0);//等待接收
        // if(ret < 0){
        //     perror("msgrcv");
        //     return -1;
        // }
        
        printf("recv msg = %s\n", mymsg.buf);
        num++;
        // break;
    }
    
    //删除消息队列
    ret = msgctl(msgid, IPC_RMID, NULL);
    if(ret < 0){
        perror("msgctl");
        return -1;
    }
    return 0;
}