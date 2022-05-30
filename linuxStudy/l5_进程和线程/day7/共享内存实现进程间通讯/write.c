#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int fd;
    // ("test", )
    fd = open("test", O_RDWR);//可以用只读方式打开  但是不能对磁盘进行写

    if(fd < 0){
        perror("open");
        return -1;
    }
    int len = lseek(fd, 0, SEEK_END);
    void* addr = mmap(NULL, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//这里的偏移量需要为0或者4K的倍数
    if(addr == MAP_FAILED){
        perror("mmap");
        return -1;
    }
    close(fd);//close之后仍然可以更改

    int i = 0;
    while(i < 2048){
        memcpy((addr+i), "a", 1);
        i++;
        sleep(1);
    }
    free(addr);
}