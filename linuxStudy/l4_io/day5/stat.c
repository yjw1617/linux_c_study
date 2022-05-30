#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
int main(){
    struct stat buf = {0};
    int ret = stat("./testFile/1.txt", &buf);
    if(ret < 0){
        perror("stat");
        return -1;
    }
    if(S_ISREG(buf.st_mode)){
        printf("-");
    }
    if(S_ISDIR(buf.st_mode)){
        printf("d");
    }
    if(S_ISCHR(buf.st_mode)){
        printf("c");
    }
    if(S_ISBLK(buf.st_mode)){
        printf("b");
    }
    if(S_ISFIFO(buf.st_mode)){
        printf("p");
    }
    if(S_ISSOCK(buf.st_mode)){
        printf("s");
    }

    int i = 0;
    for(i = 8; i >= 0; i--){
        if(buf.st_mode & (1 << i)){
            switch(i % 3){
                case 2:
                    printf("r");
                    break;
                case 1:
                    printf("w");
                    break;
                case 0:
                    printf("x");
                    break;
                default:
                    break;
            }
        }else{
            printf("-");
        }
    }


    printf("  %d",(int)buf.st_size);

    struct tm *t;

    t = localtime(&(buf.st_ctime));

    printf(" %d-%d-%d %d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
    
    printf(" 1.txt\n");
    return 0;
}