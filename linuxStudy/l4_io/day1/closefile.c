#include <stdio.h>
int main(){
	FILE* fp = fopen("2.txt","w" );
	if(fp == NULL){
	//	printf("file open error\n");
		perror("fopen:");//标准错误
	}else{
		printf("file open success\n");
		if(fclose(fp) == 0){
			printf("file close success\n");
		}else{
			printf("file close error\n");
			perror("close");
		}
	}
	return 0;
}
