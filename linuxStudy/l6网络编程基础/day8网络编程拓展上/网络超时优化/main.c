#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>	
#include <string.h>
#include <arpa/inet.h>
void* recv_client_task(void* param){
    int cfd = *(int*)param;
    char buf[20] = {0};
    int ret;
    while(1){
        while(read(cfd, buf, 20) == 0){
            exit(0);
        }
        printf("read data = %s\n", buf);
        memset(buf, 0, 20);
    }
}
int main(int argc, char** argv){
    int fd;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        return -1;
    }
    //优化：允许绑定地址快速重用
    int b_reuse = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));//设置socket可以快速重用
    int b_br = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &b_br, sizeof(int));//设置socket可以广播
    
    
    
    
    
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &b_br, sizeof(int));//设置socket可以广播
    struct timeval tout;
    tout.tv_sec = 5;
    tout.tv_usec = 0;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tout, sizeof(tout));//设置接收超时时间为5秒
    //绑定ip和端口号
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    
    
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perror("bind");
        return -1;
    }

    //打开监听
    ret = listen(fd, 5);
    if(ret < 0){
        perror("listen");
        return -1;
    }

    //阻塞着接收用户
    int cfd;
    struct sockaddr_in cin;
    int len = sizeof(cin);
    cfd = accept(fd, (struct sockaddr*)&cin, &len);
    if(cfd < 0){
        perror("accept");
        return -1;
    }
    char ipv4_addr[16];
    if(!inet_ntop(AF_INET, &cin.sin_addr.s_addr, ipv4_addr, sizeof(cin))){
        perror("inet_ntop");
        exit(1);
    }
    printf("client %s is comming\n",ipv4_addr);
    //创建线程接收用户的信息
    pthread_t tid;
    pthread_create(&tid, NULL, recv_client_task, (void*)&cfd);
    char buff_w[20] = "hello";
    while(1){
        printf("please input msg to client:\n");
        //写
        fgets(buff_w, sizeof(buff_w), stdin);
        write(cfd, buff_w, strlen(buff_w));
    }
    return 0;


}