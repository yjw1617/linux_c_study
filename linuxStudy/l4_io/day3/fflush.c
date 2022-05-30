#include <stdio.h>
#include <unistd.h>
int main(){

	FILE* fp;
	fp = fopen("test.txt", "w");
	if(fp == NULL){
		perror("fopen:");
		return -1;
	}
	fwrite("abcdef\n", 7, 1, fp);
	fflush(fp);
	while(1){
		sleep(1);
	}
	return 0;
}
