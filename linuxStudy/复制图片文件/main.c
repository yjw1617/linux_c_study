#include <stdio.h>
#include <unistd.h>

int main(){
    FILE *fp1, *fp2;
    char* r_buf[100];
    
    char cfilename[100];
    char filename[100];
    printf("please input copy file name:\n");
    scanf("%s", cfilename);
    getchar();

    printf("please input file name:\n");
    scanf("%s", filename);
    getchar();

    fp1 = fopen(cfilename, "r");

    fp2 = fopen(filename, "w");

    while(fread(r_buf, 1, 100, fp1) > 0){
        fwrite(r_buf, 1, 100, fp2);
    }
}