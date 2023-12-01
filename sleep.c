//sleep 程序的目的是让调用它的进程暂停执行指定的“ticks”数量
#include "user/user.h"
#include "kernel/types.h"
#include "kernel/stat.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        int n = atoi(argv[1]);
        sleep(n);
    }     
    if (argc == 0){
        printf("arguments error"); 
        exit(1);              
    } 

    exit(0);
}
