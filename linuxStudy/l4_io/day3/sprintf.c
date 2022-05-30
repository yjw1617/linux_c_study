#include <stdio.h>
int main(){

	FILE* fp = fopen("test.txt", "w");
	
	fprintf(fp, "nihao %s", "yjw");
	char name[10];
	sprintf(name, "hello %s\n", "ni");
	printf("%s\n", name);
	fclose(fp);
	return 0;
}
