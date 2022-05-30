#include <stdio.h>
struct student{
	char name[16];
	int age;
	char sex[8];
};
int main(){
	char buff[100];
	struct student stu = {0};
	FILE* fp = fopen("test.txt", "r");
	if(fp == NULL){
		perror("open");
		return -1;
	}
	if(fread(&stu, 100, 1, fp) == EOF){
		perror("fread");
		return -1;
	}
	printf("read data = %s\n", stu.name);
	printf("read data = %d\n", stu.age);
	printf("read data = %s\n", stu.sex);
	
	fclose(fp);
	return 0;
}
