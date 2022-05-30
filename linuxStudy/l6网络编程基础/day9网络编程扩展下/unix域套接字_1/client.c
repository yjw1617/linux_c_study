#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
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
    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        return -1;
    }

    //绑定ip
    struct sockaddr_un sun={
        AF_LOCAL,
        "/tmp/my_domain_file.1",//这个文件要求先存在并且可写
    };

    //确保文件存在并且可写
    if(access("/tmp/my_domain_file.1", F_OK | W_OK) < 0){//文件存在就删除
        exit(1);
    }
    
    //连接服务器
    if(connect(fd, (struct sockaddr*)&sun, sizeof(sun)) == -1){
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