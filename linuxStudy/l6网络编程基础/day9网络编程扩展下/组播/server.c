#include <pthread.h>
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
int main(){
    int fd;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1){
        perror("socket");
        return -1;
    }
    
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    //加入到组播
    struct ip_mreq mreq;
    bzero(&mreq, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr("235.10.10.3");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    

    //绑定ip和端口号
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8999);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perror("bind");
        return -1;
    }
    char buf[50];
    struct sockaddr_in cin;
    socklen_t addrlen = sizeof(cin);
    while(1){
        memset(buf, 0 , 50);
        if(recvfrom(fd, buf, 50, 0, (struct sockaddr*)&cin, &addrlen) < 0){
            perror("recvform");
            break;
        }
        char ipv4_addr[16];
        if(!inet_ntop(AF_INET, &cin.sin_addr.s_addr, ipv4_addr, sizeof(cin))){
            perror("inet_ntop");
            break;
        }
        printf("Recive from braodcast (%s:%d) data: %s\n", ipv4_addr, ntohs(cin.sin_port), buf);
        //发送消息回去
        if(-1 ==sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&cin, sizeof(cin))){
            printf("send msg error\n");
        }
    }
    
}