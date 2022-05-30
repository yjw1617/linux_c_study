#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
//INADDY_ANY 相当于0
void* recv_client_task(void* param){
    int cfd = *(int*)param;
    char buf[20] = {0};
    int ret;
    while(1){
        ret = read(cfd, buf, 20);
        if(ret == -1){
            perror("ret = -1");
            exit(1);
        }
        if(ret == 0){
            perror("ret = 0");
            exit(1);
        }
        printf("read data = %s\n", buf);
        memset(buf, 0, 20);
    }
}
int main(){
    int fd;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        return -1;
    }

    //绑定ip和端口号
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    
    
    //连接服务器
    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("connect error");
        return -1;
    }else{
        printf("connect success\n");
    }


    //创建线程接收用户的信息
    pthread_t tid;
    pthread_create(&tid, NULL, recv_client_task, (void*)&fd);
    char buf_w[20] = "hello";
    while(1){
        printf("please input msg to server:\n");
        fgets(buf_w, sizeof(buf_w), stdin);
        //写
        write(fd, buf_w, strlen(buf_w));
    }

    close(fd);
    return 0;


}