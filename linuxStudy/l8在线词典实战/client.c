#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>//sockaddr_in
#include <netdb.h>//gethostbyname域名解析
#include <string.h>
#include <unistd.h>
#define NAMELEN 30 //name的长度
typedef enum{
    R = 1,  // user - register
    L,      // user - login
    Q,      // user - query
    H,      // user - history
}User_select; 
//定义通信双方的信息结构体 两者必须保持一致
typedef struct{
    int type;
    char name[NAMELEN];
    char data[256];
}MSG;


int user_register(int sockfd, MSG* msg){
    printf("register:\n");
    msg->type = R;//注册消息
    printf("Input name:");
    scanf("%s", msg->name);//注册名字
    getchar();
    
    printf("Input password:");
    scanf("%s", msg->data);//注册密码
    getchar();

    if(write(sockfd, msg, sizeof(MSG)) < 0){//发送消息给服务器
        printf("fail to send\n");
        exit(-1);
    }

    if(read(sockfd, msg, sizeof(MSG)) < 0){
        printf("faild to recv");
        exit(-1);
    }

    //if ok or err
    printf("%s\n", msg->data);
    return 0;
}

int user_login(int sockfd, MSG* msg){
    printf("login:\n");
    msg->type = L;//注册消息
    printf("Input name:");
    scanf("%s", msg->name);//注册名字
    getchar();
    
    printf("Input password:");
    scanf("%s", msg->data);//注册密码
    getchar();

    if(write(sockfd, msg, sizeof(MSG)) < 0){//发送消息给服务器
        printf("fail to send\n");
        return -1;
    }

    if(read(sockfd, msg, sizeof(MSG)) < 0){
        printf("faild to recv");
        return -1;
    }

    if(strncmp(msg->data, "OK", 2) == 0){
        printf("login success!\n");
        return 1;
    }
    printf("login failed!\n");
    //if ok or err
    return 0;
}

int query_word(int sockfd, MSG* msg){
    msg->type = Q;
    puts("----------------");

    while(1){
        printf("Input word:");
        scanf("%s", msg->data);
        getchar();
        if(strncmp(msg->data, "cd", strlen("cd")) == 0){
            break;
        }

        write(sockfd, msg, sizeof(MSG));

        if(read(sockfd, msg, sizeof(MSG)) < 0){
            printf("failed to recv\n");
            return -1;
        }
        printf("%s\n", msg->data);//打印服务器返回的信息
    }
}

int query_history(int sockfd, MSG* msg){
    msg->type = H;
    write(sockfd, msg, sizeof(MSG));
    //接收服务器传递回来的历史记录
    while(1){
        if(read(sockfd, msg, sizeof(MSG)) <= 0){
            return 0;
        }

        if(msg->data[0] == '\0'){
            break;
        }
        if(strncmp(msg->data, "end", strlen("end")) == 0){
            return 0;
        }
        printf("%s\n", msg->data);
        
    }
    return 0;
}
// ./server 192.168.x.xxx 10000
int main(int argc, const char* argv[]){
    int n = 0;//主页用户输入的选项
    MSG msg;
    if(argc != 3){
        printf("Usage:%s serverip port.\n", argv[0]);
        return -1;
    }

    //创建一个socket
    int fd;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        perror("socket");
        exit(-1);
    }

    //域名解析
    // struct hostent* host = NULL;
    // host = gethostbyname(argv[1]);
    // if(!host){
    //     herror("gethostbyname");
    //     return 0;
    // }
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    //连接服务器
    if(connect(fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
        perror("connect");
        exit(-1);
    }
    while(1){
        printf("*******************************\n");
        printf("1.register     2.login   3.quit\n");
        printf("*******************************\n");
        printf("Please choose:\n");
        scanf("%d", &n);
        getchar();

        switch (n)
        {
        case 1:
            user_register(fd, &msg);
            /* code */
            break;
        case 2:
            if(user_login(fd, &msg)){
                goto next;
            }
            break;
        case 3:
            close(fd);
            exit(0);
            break;
        default:
            printf("Invalid data cmd\n");
            break;
        }
    }

next:
    while(1){
        printf("*****************************************\n");
        printf("1.query word     2.history record  3.quit\n");
        printf("*****************************************\n");
        printf("Please choose:\n");
        scanf("%d", &n);
        getchar();

        switch (n)
        {
        case 1:
            /* code */
            query_word(fd, &msg);
            break;
        case 2:
            /* code */
            query_history(fd, &msg);
            break;
        case 3:
            /* code */
            close(fd);
            exit(0);
            break;
        default:
            printf("invalid data cmd\n");
            break;
        }
    }
    return 0;
}