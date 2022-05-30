#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>

#include <sys/time.h>
//INADDY_ANY 相当于0
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
    char buf_w[20] = "hello";
    // while(1){
    //     printf("please input msg to server:\n");
    //     fgets(buf_w, sizeof(buf_w), stdin);
    //     write(fd, buf_w, strlen(buf_w));
    // }
    fd_set rset;
    int maxfd = -1;
    struct timeval tout;
    char buf[50];
    int ret = 0;
    while(1){
        FD_ZERO(&rset);
        FD_SET(0, &rset);//标准输入
        FD_SET(fd, &rset);//新用户到达

        maxfd = fd;

        tout.tv_sec = 5;
        tout. tv_sec = 0;
        select(maxfd + 1, &rset, NULL, NULL, &tout);
        if(FD_ISSET(0, &rset)){
            //读取键盘输入  发送到网络套接子
            bzero(buf, 50);
            if(fgets(buf, 50, stdin) < 0){
                continue;
            }
            if(write(fd, buf, strlen(buf)) < 0){
                perror("write to spcket");
                return 0;
            }
        }
        if(FD_ISSET(fd, &rset)){//服务器发送过来数据  读取套芥子数据 处理
            bzero(buf, 50);
            ret = read(fd, buf, 50);
            if(ret < 0){
                perror("read");
            }
            if(ret == 0){
                break;
            }
            printf("server msg: %s\n", buf);
        }
    }
    close(fd);
    return 0;
}