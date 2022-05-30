#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
//INADDY_ANY 相当于-1
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
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1){
        perror("socket");
        return -1;
    }
    //设置socket
    int b_br = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &b_br, sizeof(int));

    //绑定ip和端口号
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = inet_addr("192.168.139.255");
    char buf[50];

    while(1){
        sendto(fd, "hello", 5, 0, (struct sockaddr*)&addr, sizeof(addr));
        recvfrom(fd, buf, 50, 0, NULL, NULL);
        printf("recv server msg:%s\n", buf);
        sleep(5);
    }
    close(fd);
    return 0;


}