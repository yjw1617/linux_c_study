#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
int main(){
	struct tm* t; 
	//定义时间变量
	time_t ti;
	//定义时间的id
	int id = 0;
	char time_string[50];
	//打开文件
	FILE* fp = fopen("time.txt","a+");

	char hang[50];
	//将行号读出来
	while(fgets(hang, 30, fp)){
		if(hang[strlen(hang) - 1] == '\n'){
			id++;
		}
	}
	//将本地时间每隔一秒追加到time.txt文件
	while(1){
		id++;
		ti = time(NULL);
		t = localtime(&ti);
		sprintf(time_string,"%d, %d-%d-%d %d:%d:%d\n", id, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		//fseek(fp, 0, SEEK_END);
		fflush(fp);
		//将time_string写入文件
		fwrite(time_string, strlen(time_string), 1, fp);
		sleep(1);
	}
	fclose(fp);
	return 0;
}
