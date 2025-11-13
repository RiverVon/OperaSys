#include <stdio.h>      // 标准输入输出
#include <string.h>     // 字符串处理函数
#include <sys/socket.h> // 套接字编程接口
#include <arpa/inet.h>  // 互联网地址转换函数
#include <unistd.h>     // POSIX操作系统API（如close）

#define PORT 8081       // 目标服务器端口号

int main() {
    int sock = 0;               // 客户端套接字文件描述符
    struct sockaddr_in serv_addr; // 服务器地址结构体
    char *message = "Hello, server!"; // 要发送的消息

    /* 创建TCP套接字
     * AF_INET: IPv4协议族
     * SOCK_STREAM: 流式套接字(TCP)
     * 0: 默认协议 */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1; // 套接字创建失败
    }

    /* 配置服务器地址 */
    serv_addr.sin_family = AF_INET;       // IPv4地址族
    serv_addr.sin_port = htons(PORT);     // 端口号(转换为网络字节序)

    /* 将IP地址从字符串转换为二进制形式
     * "127.0.0.1": 本地回环地址
     * 返回值 <= 0 表示转换失败 */
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    /* 连接到服务器
     * 阻塞直到连接成功或失败 */
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    /* 发送数据到服务器
     * strlen(message): 消息长度
     * 0: 默认标志位 */
    send(sock, message, strlen(message), 0);
    printf("Message sent to server\n");

    /* 关闭套接字 */
    close(sock);
    return 0;
}
