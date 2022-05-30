#include <stdio.h>
#include <string.h>
int main(){
	FILE* fp = fopen("test.txt","r");
	if(fp == NULL){
	//	printf("file open error\n");
		perror("fopen:");//标准错误
		return 0;
	}else{
		printf("file open success\n");
	}
	int a = 0;
	while(1){
		a = fgetc(fp);
		if(a == -1){
			break;
		}
		printf("%c", a);
	}
	printf("\n");
	if(fp != NULL){
		fclose(fp);
		printf("close file success\n");
	}else{
		perror("close:");
		return -1;
	}


	//写数据入文件
	
	fp = fopen("test.txt","a");
	if(fp == NULL){
	//	printf("file open error\n");
		perror("fopen:");//标准错误
		return 0;
	}else{
		printf("file open success\n");
	}
	char* w_buff = "hello\n";
	for(int i = 0; i < strlen(w_buff); i++){
		fputc(w_buff[i], fp);
	}
	if(fp != NULL){
		fclose(fp);
		printf("close file success\n");
	}else{
		perror("close:");
		return -1;
	}
	return 0;
}
