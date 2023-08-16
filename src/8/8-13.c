// TODO need finish TELL and WAIT
#include "apue.h"

static void char_at_atime(char*);

int main(void) {
    pid_t pid;

//    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
//        WAIT_PARENT();
        char_at_atime("output from child\n");
        // TELL_PARENT(getppid());
    } else {
        // WAIT_CHILD();
        char_at_atime("output from parent\n");
//        TELL_CHILD(pid);
    }
    exit(0);
}

static void char_at_atime(char* str) {
    char *ptr;
    int c;

    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++) != 0;) {
        putc(c, stdout);
    }
}
