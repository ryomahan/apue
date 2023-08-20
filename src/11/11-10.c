/**
 * 保护某个 struct
 * foo_alloc: 创建 struct 时，设置计数器和锁，并将锁初始化
 * foo_hold: 使用 struct 时，调用此函数，加锁并且引用数加一
 * foo_rele: 使用完毕，引用数减一，查看当前引用数如果为零释放 struct，否则解锁
 */
#include "apue.h"
#include <pthread.h>


struct foo {
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};

struct foo* foo_alloc(int id) {
    struct foo *fp;

    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return (NULL);
        }
    }

    return (fp);
}

void foo_hold(struct foo *fp) {
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(struct foo *fp) {
    pthread_mutex_lock(&fp->f_lock);
    if (--fp->f_count == 0) {
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&fp->f_lock);
    }
}
