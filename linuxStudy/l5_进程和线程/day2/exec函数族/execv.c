#include <stdio.h>
#include <unistd.h>

int main(){
    char *arg[] = {NULL};
    if(execv("./test", arg) < 0){
        perror("exec");
    }
    return 0;
}