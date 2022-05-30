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
    // ret = mkfifo("./myfifo", 666);
    // if(ret < 0){
    //     perror("mkfifo");
    //     return -1;
    // }
    fp = open("./myfifo", O_WRONLY);
    if(fp < 0){
        perror("open");
        return -1;
    }
    while(1){
        memset(buf, 0, 32);
        fgets(buf, 32, stdin);
        write(fp, buf, 32);
    }
    return 0;
}