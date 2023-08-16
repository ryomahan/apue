#include "apue.h"

int main(void) {
    printf("ruid = %d, euid = %d\n", getuid(), geteuid());
    return 0;
}