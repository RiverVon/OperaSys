// 包含所需的头文件
#include <iostream>     // 用于 C++ 的标准输入输出 (std::cout, std::endl)
#include <queue>        // 用于 C++ 的队列 (std::queue)
#include <pthread.h>    // 用于 POSIX 线程
#include <unistd.h>     // 用于 sleep 函数

// 定义缓冲区的固定大小
#define BUFFER_SIZE 5

// --- 全局变量 ---
std::queue<int> buffer;     // 使用 C++ STL 的 queue 作为共享缓冲区
pthread_mutex_t mutex;    // 互斥锁：用于保护对缓冲区的访问
pthread_cond_t cond_full;  // 条件变量：当缓冲区"满"时，生产者在此等待
pthread_cond_t cond_empty; // 条件变量：当缓冲区"空"时，消费者在此等待

// --- 生产者线程函数 ---
void* producer(void* param) {
    for (int i = 0; i < 10; i++) {
        int item = rand() % 100; // 生产一个随机物品

        // 1. 进入管程（获取互斥锁）
        pthread_mutex_lock(&mutex);

        // 2. 检查条件：缓冲区是否满了？
        // 重点：必须使用 while 循环检查！
        // 因为当线程被唤醒时，它所等待的条件可能又被其他线程改变了（虚假唤醒或多个线程被唤醒）
        while (buffer.size() >= BUFFER_SIZE) {
            std::cout << "缓冲区满了，生产者 " << pthread_self() << " 正在等待...\n";
            // 缓冲区满了：
            // 1) pthread_cond_wait 会原子地 "释放 mutex 锁" 并 "使当前线程阻塞在 cond_empty 上"
            // 2) 当被唤醒时，它会重新尝试获取 mutex 锁，然后再从 wait 返回
            pthread_cond_wait(&cond_empty, &mutex);
        }

        // 3. 执行操作（此时已持有锁且缓冲区未满）
        buffer.push(item); // 将物品放入队列
        std::cout << "Produced: " << item << " (缓冲区大小: " << buffer.size() << ")\n";

        // 4. 通知等待的消费者
        // 唤醒一个可能因为缓冲区为空而正在 cond_full 上等待的消费者
        pthread_cond_signal(&cond_full);

        // 5. 退出管程（释放互斥锁）
        pthread_mutex_unlock(&mutex);

        sleep(1); // 模拟生产耗时
    }
    return nullptr;
}

// --- 消费者线程函数 ---
void* consumer(void* param) {
    for (int i = 0; i < 10; i++) {
        
        // 1. 进入管程（获取互斥锁）
        pthread_mutex_lock(&mutex);

        // 2. 检查条件：缓冲区是否为空？
        // 同样，必须使用 while 循环
        while (buffer.empty()) {
            std::cout << "缓冲区空了，消费者 " << pthread_self() << " 正在等待...\n";
            // 缓冲区空了：
            // 1) 释放 mutex 锁，并阻塞在 cond_full 上
            // 2) 被唤醒后，重新获取 mutex 锁
            pthread_cond_wait(&cond_full, &mutex);
        }

        // 3. 执行操作（此时已持有锁且缓冲区不为空）
        int item = buffer.front(); // 从队列头部获取物品
        buffer.pop(); // 移除物品
        std::cout << "Consumed: " << item << " (缓冲区大小: " << buffer.size() << ")\n";

        // 4. 通知等待的生产者
        // 唤醒一个可能因为缓冲区已满而正在 cond_empty 上等待的生产者
        pthread_cond_signal(&cond_empty);

        // 5. 退出管程（释放互斥锁）
        pthread_mutex_unlock(&mutex);

        sleep(1); // 模拟消费耗时
    }
    return nullptr;
}

// --- 主函数 ---
int main() {
    pthread_t prod_thread, cons_thread;

    // --- 初始化互斥锁和条件变量 ---
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond_full, nullptr);
    pthread_cond_init(&cond_empty, nullptr);

    // --- 创建线程 ---
    pthread_create(&prod_thread, nullptr, producer, nullptr);
    pthread_create(&cons_thread, nullptr, consumer, nullptr);

    // --- 等待线程结束 ---
    pthread_join(prod_thread, nullptr);
    pthread_join(cons_thread, nullptr);

    // --- 销毁互斥锁和条件变量 ---
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    return 0;
}