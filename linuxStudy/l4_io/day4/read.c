#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int fd;
	fd = open("1.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);
	if(fd < 0){
		printf("open file error\n");
		return 0;
	}
	printf("open file success!\n");

    //写入内容
    write(fd, "hello\n", sizeof("hello\n") - 1);

    //直接读的话读不出来的   因为此时指针已经在文件末尾

	int ret = close(fd);
	if(ret == 0){
		printf("close file success\n");
	}else{
		printf("close file failed\n");
	}

    char rd_buff[30] = {0};
    fd = open("1.txt", O_RDWR, 0666);
    read(fd, rd_buff, 30);
    printf("read data is %s\n", rd_buff);
    close(fd);
	return 0;
}