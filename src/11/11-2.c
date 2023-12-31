#include "apue.h"
#include <pthread.h>


pthread_t ntid;

void print_ids(const char * s) {
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long )tid);
}

void* thr_fn(void * arg) {
    print_ids("new thread: ");
    return((void *)0);
}

int main(void) {
    int err;
    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread");
    }
    print_ids("main thread:");
    printf("%lu\n", (unsigned long)ntid);
    sleep(1);
    exit(0);
}
