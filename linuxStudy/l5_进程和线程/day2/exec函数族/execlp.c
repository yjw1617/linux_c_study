#include <stdio.h>
#include <unistd.h>

int main(){
    if(execlp("ls", "-a", "-l", "./", NULL) < 0){
        perror("exec");
    }
    printf("after\n");//不会执行该命令   因为execlp的进程代替了原来的进程
    return 0;
}