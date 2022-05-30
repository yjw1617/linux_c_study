#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(){
    int pfd[2];
    int ret;
    pid_t pid;
    char buf[20];
    ret = pipe(pfd);
    if(ret < 0){
        perror("pipe");
        return -1;
    }
    pid = fork();
    if(pid < 0){
        perror("fork");
        return -1;
    }else if(pid == 0){
        while(1){
            strcpy(buf, "hahahhahahah");
            write(pfd[1], buf, strlen(buf));
            sleep(1);
        }
    }else{
        while(1){
            ret = read(pfd[0], buf, 20);
            if(ret > 0){
                printf("read pipe = %s\n", buf);
            }
        }
    }
    return 0;
}