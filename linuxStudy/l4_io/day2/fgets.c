#include <stdio.h>

int main(){
	FILE* fp;
	char* ret;
	char buff[100];
	fp = fopen("2.txt", "a+");
	if(fp == NULL){
		perror("openfile");
		return -1;
	}
	
	ret = fgets(buff,100,fp);//这里会等待终端输入内容
	if(ret == NULL){
		perror("fgets");
		fclose(fp);
		return 0;
	}
	printf("buff = %s\n", buff);
	return 0;
}
