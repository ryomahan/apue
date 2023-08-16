#include "apue.h"
#include <sys/wait.h>

char* env_init[] = {"USER=unknow", "PATH=/Users/ryoma/project/learn/learn-csapp/target", NULL};

int main(void) {
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        printf("execle:\n");
        if (execle("/Users/ryoma/project/learn/learn-csapp/target/8-17", "echoall", "myarg1", "MYARG2", (char*)0, env_init) < 0) {
            err_sys("excle error");
        }
    }

    if  (waitpid(pid, NULL, 0) < 0) {
        err_sys("wait error");
    }

    if ((pid = fork()) < 0) {
        err_sys("wait error");
    } else if (pid == 0) {
        printf("execlp:\n");
        if (execlp("target/8-17", "echoall", "only 1 arg", (char *) 0) < 0) {
            err_sys("execlop error");
        }
    }

    exit(0);
}

