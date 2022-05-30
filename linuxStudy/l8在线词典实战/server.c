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


#include <signal.h>
#include <sqlite3.h>

#include <sys/types.h>
#include <sys/wait.h>


#include <time.h>
#define NAMELEN 30 //name的长度
#define DATABASE "my.db"

typedef struct{
    int type;
    char name[NAMELEN];
    char data[256];
}MSG;

typedef enum{
    R = 1,  // user - register
    L,      // user - login
    Q,      // user - query
    H,      // user - history
}User_select; 


void sig_handle(int sig){
    if(sig == SIGCHLD){
        wait(NULL);//替子进程收尸
    }
}

void do_register(int sockfd, MSG* msg, sqlite3* db){
    char* errmsg;
    char sql[128];
    sprintf(sql, "insert into user values('%s', '%s');", msg->name, msg->data);
    printf("%s\n", sql);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK){
        printf("%s\n", errmsg);
        strcpy(msg->data, "user name already exist!");
        printf("client register failed!\n");
    }else{
        printf("client register ok!\n");
        strcpy(msg->data, "register success!");
    }

    if(write(sockfd, msg, sizeof(MSG)) < 0){
        printf("send to client");
    }
}

int do_login(int sockfd, MSG* msg, sqlite3* db){
    char* errmsg;
    char sql[128];
    char** resultp;
    int row;//行数
    int column;//列数
    sprintf(sql, "select * from user where name = '%s' and pass = '%s';", msg->name, msg->data);
    if(sqlite3_get_table(db, sql, &resultp, &row, &column, &errmsg) != SQLITE_OK){
        printf("%s\n", errmsg);
    }
    if(row == 1){
        sprintf(msg->data, "OK");
    }else{
        sprintf(msg->data, "FAIL");
    }
    if(write(sockfd, msg, sizeof(MSG)) < 0){
        printf("write err");
        return -1;
    }
    return 0;
}
int history_callback(void* arg, int f_num, char** f_value, char** f_name){
    int sockfd;
    MSG msg;
    sockfd = *(int*)arg;
    sprintf(msg.data, "%s, %s", f_value[1], f_value[2]);
    write(sockfd, &msg, sizeof(MSG));
    return 0;
}   

int do_history(int sockfd, MSG* msg, sqlite3* db){
    char sql[128] = {};
    char** resultp;
    int row;//行数
    int column;//列数
    char* errmsg;
    sprintf(sql, "select * from record where name = '%s'", msg->name);
    //查询数据库
    if(sqlite3_exec(db, sql, history_callback, (void*)&sockfd, &errmsg)){
        printf("exec err %s\n", errmsg);
    }else{
        printf("query success\n");
    }
    //发送一个结束标志
    strcpy(msg->data, "end");
    write(sockfd, msg, sizeof(MSG));
}


int do_searchword(MSG* msg, int sockfd, char* word){
    //打开文件  读取文件  进行比对
    FILE* fp;
    if((fp = fopen("dict.txt", "r")) == NULL){
        perror("fopen");
        return -1;
    }
    //打印提示
    int len = strlen(word);
    printf("%s, len = %d\n", word, len);


    //读文件查询单词
    int ret = 0;
    char buf[400] = {0};
    char* p;
    while(fgets(buf, 400, fp)){
        ret = strncmp(word, buf, len);
        if(ret > 0){
            continue;
        }
        if(ret < 0 || buf[len] != ' '){
            break;
        }
        p = buf + len;
        while(*p == ' '){
            p++;
        }
        //找到注释
        strcpy(msg->data, p);
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}
int get_date(char* date){
    time_t t;
    time(&t);
    //时间的格式转换
    struct tm* ti = localtime(&t);
    sprintf(date, "%d-%d-%d %d:%d:%d", ti->tm_year+1900, ti->tm_mon+1,ti->tm_mday,ti->tm_hour, ti->tm_min, ti->tm_sec);
    return 1;
}
int do_query(int sockfd, MSG* msg, sqlite3* db){
    char word[64];
    int found = 0;
    char date[128];//时间
    char sql[128] = {0};
    char* errmsg = NULL;
    //将客户端发送过来的单词拿出
    // printf("client word =%s\n", msg->data);
    strcpy(word, msg->data);
    
    // printf("word = %s\n", word);
    found = do_searchword(msg, sockfd, word);

    //找到了单词,此时将用户名,时间,单词,插入到历史记录表中
    if(found == 1){
        //需要获取系统时间
        get_date(date);
        sprintf(sql, "insert into record values('%s', '%s', '%s');", msg->name, date, word);

        if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK){
            printf("%s\n", errmsg);
            return -1;
        }else{
            printf("Insert record done\n");
        }
    }else{
        printf("not find word\n");
        strcpy(msg->data, "Not found");
    }
    write(sockfd, msg, sizeof(MSG));
}
int do_client(int acceptfd, sqlite3* db){
    MSG msg;
    while(read(acceptfd, &msg, sizeof(MSG)) > 0){
        switch (msg.type)
        {
        case R:
            /* code */
            do_register(acceptfd, &msg, db);
            break;
        case L:
            /* code */
            do_login(acceptfd, &msg, db);
            break;
        case H:
            /* code */
            do_history(acceptfd, &msg, db);
            break;
        case Q:
            /* code */
            printf("QQQQ\n");
            do_query(acceptfd, &msg, db);
            break;
        default:
            printf("invalid data\n");
            break;
        }
    }
    printf("client exit\n");
    close(acceptfd);
    // exit(0);

    return 0;
}
int main(int argc, char** argv){
    int fd;

    sqlite3* db;//数据库句柄

    
    if(argc != 3){
        printf("Usage:%s serverip port.\n", argv[0]);
        return -1;
    }
    //打开数据库
    if(sqlite3_open(DATABASE, &db) != SQLITE_OK){
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }else{
        printf("open db success\n");
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        return -1;
    }
    //优化：允许绑定地址快速重用
    int b_reuse = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));//设置socket可以快速重用
    
    //绑定ip和端口号
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if(ret < 0){
        perror("bind");
        return -1;
    }

    //打开监听，套接子又主动变被动
    ret = listen(fd, 5);
    if(ret < 0){
        perror("listen");
        return -1;
    }

    //处理僵尸进程
    signal(SIGCHLD, sig_handle);//子进程退出就会给父进程发送

    //阻塞着接收用户
    
    int acceptfd;//接收的客户端的数据句柄
    pid_t pid;//创建的进程号
    while(1){
        if(acceptfd = accept(fd, NULL, NULL) < 0){
            perror("accept");
            return -1;
        }
        //创建子进程
        pid = fork();
        if(pid < 0){
            perror("fork");
            return -1;
        }else if(pid == 0){//子进程
            close(fd);
            do_client(acceptfd, db);
        }else{//父进程  用来接收客户端的请求
            close(acceptfd);
        }
    }
    
    return 0;


}