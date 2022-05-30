#include <stdio.h>
int main(){
	FILE* fp = fopen("test.t", "a");	
	if(fp == NULL){
		perror("fopen");
		return -1;
	}else{
		printf("fopen success\n");
	}
	if(fputs("hellonihao\n", fp) == NULL){
		perror("fopen:");
		return -1;
	}else{
		printf("fputs success\n");
	}
	if(fp != NULL){
		fclose(fp);
	}
}
