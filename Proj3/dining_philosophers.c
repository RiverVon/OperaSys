// 包含所需的头文件
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>    // 用于 POSIX 线程
#include <semaphore.h>  // 用于信号量
#include <unistd.h>     // 用于 sleep 函数

// 定义哲学家的数量
#define NUM_PHILOSOPHERS 5

// --- 全局变量 ---
sem_t forks[NUM_PHILOSOPHERS]; // 信号量数组，代表 5 根筷子
pthread_t philosophers[NUM_PHILOSOPHERS]; // 线程数组，代表 5 位哲学家

// --- 哲学家线程函数 ---
void* philosopher(void* num) {
    // 将传入的 void* 参数转回为哲学家 ID (int)
    // 注意：这里直接转换了指针的值，而不是指针指向的值。
    // 这种技巧在主函数中通过 (void*)t 传递（在64位系统上）可能不安全，
    // 但在指导书的示例中，是传递 &ids[i]，所以我们用 (int*)num 获取地址，再用 * 解引用
    int id = *((int*)num);

    while (1) { // 无限循环：思考 -> 进餐

        // --- 1. 思考 ---
        printf("哲学家 %d 正在思考。\n", id);
        sleep(1); // 模拟思考 1 秒

        // --- 2. 请求筷子 (尝试进餐) ---
        // 这是避免死锁的关键：通过ID的奇偶性，改变获取筷子的顺序

        if (id % 2 == 0) {
            // 偶数号哲学家：先拿左手 (id)，再拿右手 ((id + 1) % 5)
            printf("哲学家 %d (偶数) 尝试拿左筷 %d\n", id, id);
            sem_wait(&forks[id]);
            printf("哲学家 %d (偶数) 拿到了左筷 %d，尝试拿右筷 %d\n", id, id, (id + 1) % NUM_PHILOSOPHERS);
            sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]);
        } else {
            // 奇数号哲学家：先拿右手 ((id + 1) % 5)，再拿左手 (id)
            printf("哲学家 %d (奇数) 尝试拿右筷 %d\n", id, (id + 1) % NUM_PHILOSOPHERS);
            sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]);
            printf("哲学家 %d (奇数) 拿到了右筷 %d，尝试拿左筷 %d\n", id, (id + 1) % NUM_PHILOSOPHERS, id);
            sem_wait(&forks[id]);
        }

        // --- 3. 进餐 ---
        // 此时，哲学家已同时持有两根筷子
        printf("哲学家 %d 正在进餐。\n", id);
        sleep(2); // 模拟进餐 2 秒

        // --- 4. 释放筷子 ---
        // 释放筷子的顺序无所谓
        printf("哲学家 %d 进餐完毕，放下左筷 %d 和 右筷 %d\n", id, id, (id + 1) % NUM_PHILOSOPHERS);
        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % NUM_PHILOSOPHERS]);
    }

    return NULL;
}

int main() {
    int ids[NUM_PHILOSOPHERS]; // 用于存放哲学家 ID 的数组

    // --- 初始化信号量 ---
    // 每根筷子 (fork) 都是一个信号量，初始值为 1 (表示可用)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
        ids[i] = i; // 设置 ID
    }

    // --- 创建哲学家线程 ---
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        // 创建线程，执行 philosopher 函数，并将 ids[i] 的地址作为参数传进去
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // --- 等待哲学家线程结束 ---
    // 因为 philosopher 函数是无限循环，所以主线程会永远阻塞在这里
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // --- 销毁信号量 ---
    // (理论上，由于上面的无限循环，代码不会执行到这里)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}