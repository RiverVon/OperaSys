#include <stdio.h>

// 定义进程结构体
struct Process {
    int id;             // 进程ID
    int burst_time;     // 执行时间（CPU突发时间）
    int waiting_time;   // 等待时间
    int turnaround_time;// 周转时间（完成时间-到达时间）
};

/* 计算等待时间
 * proc[]: 进程数组
 * n: 进程数量 */
void findWaitingTime(struct Process proc[], int n) {
    proc[0].waiting_time = 0;  // 第一个进程等待时间为0
    
    // 后续进程的等待时间 = 前一个进程的等待时间 + 前一个进程的执行时间
    for (int i = 1; i < n; i++) {
        proc[i].waiting_time = proc[i-1].waiting_time + proc[i-1].burst_time;
    }
}

/* 计算周转时间
 * 周转时间 = 执行时间 + 等待时间 */
void findTurnaroundTime(struct Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].burst_time + proc[i].waiting_time;
    }
}

int main() {
    // 初始化进程数组（假设所有进程同时到达）
    struct Process proc[] = {
        {1, 10},  // 进程1，执行时间10ms
        {2, 5},   // 进程2，执行时间5ms
        {3, 8}    // 进程3，执行时间8ms
    };
    
    int n = sizeof(proc) / sizeof(proc[0]);  // 计算进程数量

    // 计算时间指标
    findWaitingTime(proc, n);
    findTurnaroundTime(proc, n);

    // 打印结果表格
    printf("Processes  Burst time  Waiting time  Turnaround time\n");
    for (int i = 0; i < n; i++) {
        printf("  %d \t\t %d \t\t %d \t\t %d\n", 
               proc[i].id, 
               proc[i].burst_time, 
               proc[i].waiting_time, 
               proc[i].turnaround_time);
    }
    
    return 0;
}
