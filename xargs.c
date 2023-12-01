#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include <stddef.h>


char *get(char *buf, int max) {
    int i, cc;
    char c;

    for (i = 0; i + 1 < max;) {
        cc = read(0, &c, 1);
        if (cc < 1) {   // EOF 或读取错误
            if (i == 0) return NULL;  // 如果没有读取到任何字符，则返回 NULL
            break;
        }
        buf[i++] = c;
        if (c == '\n' || c == '\r') break;
    }
    buf[i] = '\0';
    return buf;
}

int main(int argc, char *argv[]) {
    char line[512];
    char *arg_alter[MAXARG];
    int n;

    // 复制静态参数
    for (n = 1; n < argc; n++) {
        arg_alter[n - 1] = argv[n];
    }

    while (1) {
        char *result = get(line, sizeof(line));
        if (result == NULL) break;  // 检测到 EOF，退出循环

        // 删除换行符
        char *newline = strchr(line, '\n');
        if (newline) *newline = 0;

        // 动态分配内存以存储行
        char *line_copy = malloc(strlen(line) + 1);
        if (line_copy == NULL) {
            break;  // 错误处理: 内存分配失败
        }
        strcpy(line_copy, line);

        // 将复制的行添加为参数
        arg_alter[argc - 1] = line_copy;
        arg_alter[argc] = 0; // 确保以 null 结尾

        // 创建子进程执行命令
        if (fork() == 0) {
            exec(arg_alter[0], arg_alter);
            exit(0);
        } else {
            wait(0);
            free(line_copy); // 释放分配的内存
        }
    }

    exit(0);
}
