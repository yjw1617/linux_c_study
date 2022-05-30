#include <sys/types.h>
#include <signal.h>

int main(){
    raise(11);//给自己发送11信号
    kill(9036, 11);//发送核心转储信号
}