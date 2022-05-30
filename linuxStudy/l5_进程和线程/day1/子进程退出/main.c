#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("hello world");
    exit(0);//会刷新流的缓冲区
    _exit(0);
    printf("exit after");
}