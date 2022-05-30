#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(){
	int fd;
	fd = open("1.txt", O_WRONLY|O_CREAT, 0666);
	if(fd < 0){
		printf("open file error\n");
		return 0;
	}
	printf("open file success!\n");
	return 0;
}
