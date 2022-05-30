#include <stdio.h>
struct student{
	char name[16];
	int age;
	char sex[8];
};
int main(){
	struct student stu = {
		"dageda",
		66,
		"nv",
	};
	struct student stu2;
	FILE* fp = fopen("test.txt", "w");
	if(fp == NULL){
		perror("open");
		return -1;
	}
	fwrite(&stu, sizeof(stu),1,fp);
	//将文件指针指向第一个字节
	fread(&stu2, sizeof(stu2), 1, fp);
	printf("stu2 name = %s, age = %d, sex = %s\n", stu2.name, stu2.age, stu2.sex);
	fclose(fp);
	return 0;
}
