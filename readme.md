# 项目名称

**操作系统实验**

---

## 🧪 实验内容

---

## 🔬 环境检查 (Demo)

* **文件:** `Demo.c`
* **说明:** 此程序用于检查和验证实验所需的 Linux 环境是否配置正确。

### 如何使用

1.  编译:
    ```bash
    gcc Demo.c -o demo
    ```
2.  运行:
    ```bash
    ./demo
    ```

---

## 🧪 实验一 (check)

* **文件:** `check_odd_even.sh`
* **说明:** 实验一的 Shell 脚本。

### 如何使用

1.  赋予执行权限:
    ```bash
    chmod +x check_odd_even.sh
    ```
2.  运行:
    ```bash
    ./check_odd_even.sh
    ```

---

## 🚀 实验二 (my_project)

* **目录:** `my_project/`
* **说明:** 实验二的 C 语言项目。
    * `src/`: 存放所有源代码 (`main.c`, `utils.c`, `utils.h`)。
    * `Makefile`: 用于编译整个项目。

### 如何使用

1.  进入项目目录:
    ```bash
    cd my_project
    ```
2.  编译 (使用 Makefile):
    ```bash
    make
    ```
3.  运行:
    ```bash
    ./my_project
    ```

---

## 💻 实验三 (Proj3)

* **目录:** `Proj3/`

* **说明:** 实验三，包含多个独立的 C/C++ 程序，演示进程管理与同步。

### 如何使用

由于没有顶层 Makefile，你需要单独编译和运行每个程序。请注意，涉及线程和信号量的程序在编译时必须链接 pthread 库。

1. 任务一：创建进程

    进入项目目录:
    ```bash
    cd Proj3
    ```

    编译:
    ```bash
    gcc create_process.c -o create_process
    ```

    运行：
    ```bash
    ./create_process
    ```

2. 任务二：进程通信 (Socket)

    文件:
    ```bash
     server.c, client.c
    ```

    编译:
    ```bash
    gcc server.c -o server
    gcc client.c -o client
    ```

    运行: (需要两个终端)
    ```bash
    终端 1 (先启动服务器):./server
    终端 2 (再启动客户端):./client
    ```
3. 任务三：创建线程

    文件:
    ```bash
    create_thread.c
    ```

    编译 (需要声明 -pthread):
    ```bash
    gcc create_thread.c -o create_thread -pthread
    ```

    运行:
    ```bash
    ./create_thread
    ```
4. 任务四：CPU调度 (FCFS)

    文件: 
    ```bash
    cpu_scheduling.c
    ```

    编译:
    ```bash
    gcc cpu_scheduling.c -o cpu_scheduling
    ```

    运行:
    ```bash
    ./cpu_scheduling
    ```



5. 任务五：生产者-消费者 (信号量)

    文件:
    ```bash
    producer_consumer_semaphore.c
    ```

    编译 (需要 -pthread):
    ```bash
    gcc producer_consumer_semaphore.c -o producer_consumer_semaphore -pthread
    ```

    运行:
    ```bash
    ./producer_consumer_semaphore
    ```

6. 任务六：生产者-消费者 (管程)

    文件:
    ```bash
    producer_consumer_monitor.cpp
    ```

    编译 (需要 g++ 和 -pthread):
    ```bash
    g++ producer_consumer_monitor.cpp -o producer_consumer_monitor -pthread
    ```

    运行:
    ```bash
    ./producer_consumer_monitor
    ```

7. 任务七：哲学家就餐问题

    文件:
    ```bash
    dining_philosophers.c
    ```
    编译 (需要 -pthread):
    ```bash
    gcc dining_philosophers.c -o dining_philosophers -pthread
    ```
    运行:
    ```bash
    ./dining_philosophers
    ```

(程序会持续运行，按 Ctrl + C 终止)
