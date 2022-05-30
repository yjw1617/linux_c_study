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
    fd = open("test", O_RDWR | O_CREAT, 0666);
    if(fd < 0){
        perror("open");
        return -1;
    }

    void* addr = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED){
        perror("mmap");
        return -1;
    }

    memcpy(addr, "abcdefg", 7);

    printf("read %s\n", (char*)addr);
}