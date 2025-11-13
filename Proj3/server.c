#include <stdio.h>      // 标准输入输出
#include <string.h>     // 字符串处理
#include <sys/socket.h> // 套接字编程接口
#include <netinet/in.h> // 互联网地址族定义
#include <unistd.h>     // POSIX操作系统API（如close）

#define PORT 8081       // 服务器监听端口

int main() {
    // 变量声明
    int server_fd;      // 服务器套接字文件描述符
    int new_socket;     // 客户端连接套接字
    struct sockaddr_in address; // 服务器地址结构体
    int addrlen = sizeof(address); // 地址结构体长度
    char buffer[1024] = {0}; // 数据接收缓冲区

    /* 创建TCP套接字
     * AF_INET: IPv4协议族
     * SOCK_STREAM: 流式套接字(TCP)
     * 0: 默认协议 */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed"); // 输出错误信息
        return 1; // 异常退出
    }

    /* 配置服务器地址 */
    address.sin_family = AF_INET; // IPv4地址族
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有网卡
    address.sin_port = htons(PORT); // 端口号(转换为网络字节序)

    /* 绑定套接字到指定端口 */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd); // 关闭套接字
        return 1;
    }

    /* 开始监听
     * 3: 等待连接队列的最大长度 */
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        return 1;
    }

    /* 接受客户端连接
     * 阻塞直到有客户端连接 */
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                    (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        close(server_fd);
        return 1;
    }

    /* 读取客户端数据
     * 最多读取1024字节 */
    read(new_socket, buffer, 1024);
    printf("Message from client: %s\n", buffer);

    /* 关闭连接 */
    close(new_socket); // 关闭客户端套接字
    close(server_fd);  // 关闭服务器套接字
    return 0;
}
