#include <dirent.h>
#include <stdio.h>
int main(){
    DIR* dp;
    dp =  opendir("./testFile");
    if(dp < 0){
        perror("opendir");
        return 0;
    }
    struct dirent* dt;
    while((dt = readdir(dp)) != NULL){
        printf("%s\n", dt->d_name);
    }
    closedir(dp);
}