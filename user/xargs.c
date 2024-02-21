/**
 * Write a simple version of the UNIX xargs program: 
 * its arguments describe a command to run, it reads lines from the standard input, 
 * nd it runs the command for each line, appending the line to the command's arguments. 
*/

#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int readline(char *params[MAXARG], int curparam) {
    char buf[1024];
    int cur = 0;
    // 读取一行的参数
    while (read(0, buf + cur, 1)) {
        if (cur == 1023) {
            fprintf(2, "ERROR: paramter is too long\n");
            exit(1);
        }

        if (buf[cur] == '\n') {
            break;
        }
        cur ++;
    }
    buf[cur] = 0;
    // 如果这一行没有参数了则直接返回0，用于退出检测
    if (cur == 0) return 0;

    int offset = 0;
    // 根据空格分割参数
    while(offset < cur) {
        params[curparam ++] = buf + offset; // 每个参数后面
        while(buf[offset] != ' ' && offset < cur) {
            offset ++;
        }
        while (buf[offset] == ' ' && offset < cur) {
            buf[offset ++] = 0; // 每个参数后面加0表示一个字符串的结尾
        }
    }

    return curparam;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "ERROR: paramters need two as least\n");
        exit(1);
    }

    int curparam = 0;    // 表示当前参数在参数列表中的位置  
    char *params[MAXARG];   // params表示参数列表，每一项表示一个参数
    char *command = malloc(strlen(argv[1]) + 1);
    strcpy(command, argv[1]);
    // 先讲已有参数加到参数列表当中

    for (int i = 1; i < argc; i ++) {
        params[curparam] = malloc(strlen(argv[i]) + 1);
        strcpy(params[curparam ++], argv[i]);
    }
    int cur_argc;
    while( (cur_argc = readline(params, curparam)) != 0) {
        params[cur_argc] = 0;
        if (fork() == 0) {
            exec(command, params);
            fprintf(2, "ERROR: exec fail\n");
            exit(1);
        } else {
            wait(0);
        }
    }
    
    exit(0);
}