#include "user/user.h"
#include "kernel/types.h"
#include "kernel/stat.h"

int main() {
    int p1[2]; // 父 -> 子
    int p2[2]; // 子 -> 父
    pipe(p1);
    pipe(p2);

    int n = fork();
    if (n > 0) {
        // 父进程
        close(p1[0]); // 关闭p1读取端
        close(p2[1]); // 关闭p2写入端
	char data = 'B';
        write(p1[1], &data, 1); // 向子进程写入数据
        close(p1[1]); // 关闭p1写入端

        wait(0); // 等待子进程

        char buf[8];
        int mun = read(p2[0], buf, sizeof buf); // 从子进程读取数据
        if (mun > 0) {
            printf("%d: received pong\n", getpid());
        }
        close(p2[0]); // 关闭p2读取端
    } else {
        // 子进程
        close(p1[1]); // 关闭p1写入端
        close(p2[0]); // 关闭p2读取端

        char buf[8];
        int mun = read(p1[0], buf, sizeof buf); // 从父进程读取数据
        if (mun > 0) {
            printf("%d: received ping\n", getpid());
        }
        close(p1[0]); // 关闭p1读取端
	char data = 'A';
        write(p2[1], &data, 1); // 向父进程写入数据
        close(p2[1]); // 关闭p2写入端

        exit(0);
    }
    exit(0);
}

