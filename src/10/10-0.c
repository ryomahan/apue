//
// Created by ryoma on 7/11/23.
//
#include <signal.h>

int main() {
    const char *msg = "test";
    psignal(1, msg);
    return 0;
};