#include <stdio.h>
int main(){
	FILE* fp = fopen("2.txt","r" );
	if(fp == NULL){
	//	printf("file open error\n");
		perror("fopen:");//标准错误
		return -1;
	}else{
		printf("file open success\n");
	}
	return 0;
}
