#include <stdio.h>      // 标准输入输出
#include <stdlib.h>     // 标准库函数（如exit）
#include <pthread.h>    // POSIX线程库

/* 线程函数（所有线程都会执行此函数）
 * 参数：void* threadid - 线程标识符（被转换成长整型）*/
void* print_message_function(void* threadid) {
    long tid;
    tid = (long)threadid;  // 将void*类型参数转为long类型
    printf("Thread #%ld is running\n", tid);
    pthread_exit(NULL);    // 显式退出线程（返回值NULL）
}

int main() {
    pthread_t threads[2];  // 存储两个线程的标识符
    int rc;                // 用于接收函数返回值
    long t;                // 循环计数器

    /* 创建两个线程 */
    for (t = 0; t < 2; t++) {
        printf("In main: creating thread %ld\n", t);
        
        /* 创建线程
         * &threads[t] - 存储线程ID的地址
         * NULL - 使用默认线程属性
         * print_message_function - 线程要执行的函数
         * (void*)t - 传递给线程函数的参数（将long转为void*）*/
        rc = pthread_create(&threads[t], NULL, print_message_function, (void*)t);
        
        if (rc) {  // 检查线程是否创建成功
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);  // 创建失败则退出程序
        }
    }
    
    /* 主线程退出（但不会终止子线程）
     * 如果使用return 0，会终止整个进程和所有线程 */
    pthread_exit(NULL);
}
