#include "apue.h"

static void char_at_atime(char *);

int main(void) {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        char_at_atime("output from child\n");
    } else {
        char_at_atime("output from parent\n");
    }
}

static void char_at_atime(char* str) {
    char* ptr;
    int c;

    setbuf(stdout, NULL);

    for (ptr = str; (c = *ptr++) !=0; ) {
        putc(c, stdout);
    }
}
