/**
 * 展示了两个互斥量的使用方法
 * 在同时需要两个互斥量时，总是让它们以相同的顺序加锁，这样可以避免死锁
 * 第二个互斥量维护着一个用于跟踪 foo 数据结构的散列列表
 *
 * hashlock 用于保护 fh 和 f_next
 * f_lock 用于保护对 foo 结构其他字段的访问
 */

#include <stdlib.h>
#include <pthread.h>


#define NHASH 29
#define HASH(id) (((unsigned long) id) %NHASH)

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
    struct foo *f_next;
};

struct foo *fh[NHASH];


struct foo* foo_alloc(int id) {
    struct foo *fp;
    int idx;

    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        // 初始化一个 foo 结构
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return (NULL);
        }
        // 将 foo 结构记录在 fh 中
        idx = HASH(id);
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        // 这里为什么需要给 fp 加锁
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
    }
    return (fp);
}

void foo_hold(struct foo *fp) {
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

struct foo* foo_find(int id) {
    // 从列表中找到指定 id 的 foo 结构
    struct foo *fp;

    // 查找前先锁定列表
    pthread_mutex_lock(&hashlock);
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
        // 找到后 foo 计数 + 1
        if (fp->f_id == id) {
            foo_hold(fp);
            break;
        }
    }
    pthread_mutex_unlock(&hashlock);
    return (fp);
}

// 使用完成后释放占用
void foo_rele(struct foo *fp) {
    struct foo *tfp;
    int idx;

    // 锁定要释放的 foo
    pthread_mutex_lock(&fp->f_lock);
    if (fp->f_count == 1) {
        // 如果 foo 当前计数只有一个
        pthread_mutex_unlock(&fp->f_lock);

        pthread_mutex_lock(&hashlock);
        pthread_mutex_lock(&fp->f_lock);

        // 二次确认，如果在这个过程中有其他之前被阻塞的地方增加了引用
        if (fp->f_count != 1) {
            fp->f_count--;
            pthread_mutex_unlock(&fp->f_lock);
            pthread_mutex_unlock(&hashlock);
            return;
        }

        // 二次确认通过，
        idx = HASH(fp->f_id);
        tfp = fh[idx];
        if (tfp == fp) {
            fh[idx] = fp->f_next;
        } else {
            while (tfp->f_next != fp) {
                tfp = tfp->f_next;
            }
            tfp->f_next = fp->f_next;
        }

        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        fp->f_count--;
        pthread_mutex_unlock(&fp->f_lock);
    }
}