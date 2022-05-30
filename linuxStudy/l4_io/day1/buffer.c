#include <stdio.h>
#include <unistd.h>
int main(int argc, char** argv){
	//满缓冲  等将缓冲区写满再打印输出
	for(int i = 0; i < 10000; i++){
		printf("aaa");
	}
	printf("hello world");//行缓冲,不加\n就会程序结束再输出
	int num = 0;
	while(1){
		num++;
		if(num == 5){
			break;
		}
		sleep(1);
	}
	return 0;
}
