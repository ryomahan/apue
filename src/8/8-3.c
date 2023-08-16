#include "apue.h"


int globvar = 6;

int main(void) {
    int var;
    pid_t pid;

    var = 88;
    printf("before vfork pid = %ld\n", (long)getpid());
    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    } else if (pid == 0) {
        // 如果是子进程
        globvar++;
        var++;
        // 退出子进程
        // _exit 并不执行标准 IO 缓冲区的冲洗操作
//        _exit(0);
        exit(0);
    }

    // 打印结果 pid = 75635, glob = 7, var = 89
    // 子进程的修改改动了父进程的内容 因为子进程在父进程的地址空间进行运行
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    exit(0);
}
