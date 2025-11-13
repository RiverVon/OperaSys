// 包含所需的头文件
#include <stdio.h>     
#include <stdlib.h>    
#include <pthread.h>   
#include <semaphore.h> 
#include <unistd.h>    

// 定义缓冲区的固定大小
#define BUFFER_SIZE 5

// --- 全局变量 ---
int buffer[BUFFER_SIZE]; // 共享缓冲区，一个循环队列
int in = 0;  // 生产者放入数据的索引 (队尾)
int out = 0; // 消费者取出数据的索引 (队头)

sem_t empty; // 信号量：记录缓冲区中 "空" 槽位的数量
sem_t full;  // 信号量：记录缓冲区中 "满" 槽位的数量
sem_t mutex; // 信号量：用于互斥访问缓冲区，确保同一时间只有一个线程在操作 buffer

// --- 生产者线程函数 ---
void* producer(void* param) {
    int item;
    for (int i = 0; i < 10; i++) { // 生产者总共生产 10 个物品
        item = rand() % 100; // 生产一个随机数作为物品

        // 1. 等待 "空" 槽位
        // P(empty): 如果 empty 值为 0 (缓冲区满了)，则阻塞等待
        sem_wait(&empty);

        // 2. 进入临界区 (准备修改缓冲区)
        // P(mutex): 获取互斥锁，如果为 0，则阻塞等待
        sem_wait(&mutex);

        // --- 临界区开始 ---
        buffer[in] = item; // 将物品放入缓冲区
        printf("Produced: %d (at index %d)\n", item, in);
        in = (in + 1) % BUFFER_SIZE; // 更新 "in" 索引，使用取模实现循环队列
        // --- 临界区结束 ---

        // 3. 退出临界区
        // V(mutex): 释放互斥锁，唤醒其他等待 mutex 的线程
        sem_post(&mutex);

        // 4. 通知消费者 "满" 槽位增加了一个
        // V(full): full 信号量加 1
        sem_post(&full);

        sleep(1); // 休眠 1 秒，模拟生产耗时，并方便观察
    }
    return NULL;
}

// --- 消费者线程函数 ---
void* consumer(void* param) {
    int item;
    for (int i = 0; i < 10; i++) { // 消费者总共消费 10 个物品
        
        // 1. 等待 "满" 槽位
        // P(full): 如果 full 值为 0 (缓冲区空了)，则阻塞等待
        sem_wait(&full);

        // 2. 进入临界区 (准备修改缓冲区)
        // P(mutex): 获取互斥锁
        sem_wait(&mutex);

        // --- 临界区开始 ---
        item = buffer[out]; // 从缓冲区取出物品
        printf("Consumed: %d (from index %d)\n", item, out);
        out = (out + 1) % BUFFER_SIZE; // 更新 "out" 索引，实现循环队列
        // --- 临界区结束 ---

        // 3. 退出临界区
        // V(mutex): 释放互斥锁
        sem_post(&mutex);

        // 4. 通知生产者 "空" 槽位增加了一个
        // V(empty): empty 信号量加 1
        sem_post(&empty);

        sleep(1); // 休眠 1 秒，模拟消费耗时，并方便观察
    }
    return NULL;
}

// --- 主函数 ---
int main() {
    // 声明生产者和消费者的线程 ID
    pthread_t prod_thread, cons_thread;

    
    // 1. empty 信号量：初始值设为 BUFFER_SIZE，因为刚开始缓冲区全是空的
    sem_init(&empty, 0, BUFFER_SIZE);
    
    // 2. full 信号量：初始值设为 0，因为刚开始缓冲区全是空的
    sem_init(&full, 0, 0);
    
    // 3. mutex 信号量：初始值设为 1，作为互斥锁 (二元信号量)
    sem_init(&mutex, 0, 1);

    // --- 创建线程 ---
    // pthread_create(线程ID指针, 线程属性, 线程函数, 传递给函数的参数)
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // --- 等待线程结束 ---
    // 主线程阻塞，直到指定的线程执行完毕
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // --- 销毁信号量 ---
    // 释放信号量占用的资源
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}