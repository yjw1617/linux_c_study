#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
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

    //发送的消息
    msgT mymsg = {
        1,
        "hello world!",
    };
    //发送消息动作
    int ret;
    while(1){
        ret = msgsnd(msgid, &mymsg, sizeof(msgT) - sizeof(long), 0);
        if(ret < 0){
            perror("msgsnd");
            return -1;
        }
        sleep(2);
    }
    
    return 0;
}