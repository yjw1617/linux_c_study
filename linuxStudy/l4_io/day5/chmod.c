#include <stdio.h>
#include <sys/stat.h>

int chmod(const char *pathname, mode_t mode);
int main(){
    if(chmod("./testFile/1.txt", 0777) < 0){
        perror("chmod");
    }
    return 0;
}