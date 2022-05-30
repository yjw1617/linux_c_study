#include <stdio.h>
#include <unistd.h>
int main(){

	FILE* fp;
	fp = fopen("test.txt", "w");
	if(fp == NULL){
		perror("fopen:");
		return -1;
	}
	fwrite("abcdef\n", 6, 1, fp);
	fseek(fp, 0, SEEK_SET);
	fwrite("vvv", 3, 1, fp);
	fflush(fp);
	return 0;
}
