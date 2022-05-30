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

#include <sys/un.h>

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
    fd = socket(AF_LOCAL, SOCK_STREAM, 0);//创建本地的套接子
    if(fd == -1){
        perror("socket");
        return -1;
    }

    //绑定ip和端口号
    // struct sockaddr_in addr;
    // bzero(&addr, sizeof(addr));
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(9000);
    // addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    struct sockaddr_un sun = {
        AF_LOCAL,
        "/tmp/my_domain_file.1",//这个文件事先不存在
    };
    if(!access("/tmp/my_domain_file.1", F_OK)){//文件存在就删除
        unlink("/tmp/my_domain_file.1");//删除文件
    }
    int ret = bind(fd, (struct sockaddr*)&sun, sizeof(sun));
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
    int cfd = 0;
    cfd = accept(fd, NULL, NULL);
    if(cfd < 0){
        perror("accept");
        return -1;
    }
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