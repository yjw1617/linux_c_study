#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid;
    printf("before exec\n");
    pid = fork();
    if(pid == 0){//子进程
        if(execl("/bin/ls", "ls", "-a", "-l", "./", NULL) < 0){//第二个参数不能去掉,但是不需要的时候可以随便写
            perror("exec");
        }
    }
    //这里子进程不会打印   因为
    printf("after\n");
    return 0;
}