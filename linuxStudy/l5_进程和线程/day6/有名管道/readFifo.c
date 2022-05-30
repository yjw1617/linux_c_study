#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
int main(){
    int ret;
    int fp;
    char buf[32];
    //管道不要建立在共享文档,因为windows不支持管道文件
    fp = open("./myfifo", O_RDONLY);
    if(fp < 0){
        perror("open");
        return -1;
    }
    while(1){
        memset(buf, 0, 32);
        ret = read(fp, buf, 32);
        if(ret > 0){
            printf("read fifo = %s\n", buf);
        }else if(ret == 0){
            exit(0);
        }   
    }
    return 0;
}