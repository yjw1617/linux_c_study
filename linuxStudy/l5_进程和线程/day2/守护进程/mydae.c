#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
int main(){
    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork");
        return 0;
    }else if(pid > 0){
        printf("i am demo\n");
        exit(0);
    }
    printf("before sid = %d, pid = %d, pgid=%d\n", getsid(pid), getpid(), getpgid(pid));
    //会话组组长
    if(setsid() < 0){
        exit(0);
        perror("setsid");
        exit(0);
    }
    printf("after sid = %d, pid = %d, pgid=%d\n", getsid(pid), getpid(), getpgid(pid));
    chdir("/");
    if(umask(0) < 0){
        perror("unmask");
        exit(0);
    }

    close(0);//标准输入
    close(1);//标准输出
    close(2);//标准错误不能使用
    printf("dsadas\n");
    sleep(100);
    return 0; 
}