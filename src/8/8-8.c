#include "apue.h"
#include <sys/wait.h>


int main(void) {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        pid = fork();
        if (pid < 0) {
            err_sys("fork error");
        } else if (pid > 0) {
            exit(0);
        }

        // 此处是第二个子进程的逻辑，当上面的代码块中本进程的父进程退出，本进程就属于 init 了
        // 执行完下面的逻辑后本进程会退出，init 进程会捕获到本进程的状态并对本进程进行回收
        sleep(2);
        printf("second child, parent pid = %ld\n", (long)getppid());
        exit(0);
    }

    if (waitpid(pid, NULL, 0) != pid) {
        err_sys("waitpid error");
    }

//    sleep(3);
    exit(0);
}
