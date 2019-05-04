#include "unp.h"

// 客户端进程
int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    // 网际套接字地址结构
    struct sockaddr_in servaddr; // sockaddr_in6

    // socket API
    // 创建套接字/端点endpoint,返回套接字描述符
    // 本地套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // AF_INET6
        err_sys("socket error");
    } // 包裹函数wrapper function 完成实际函数调用,检查返回值,在发生错误时终止进程(一般)
    bzero(&servaddr, sizeof(servaddr));
    // 地址族AF_INET
    servaddr.sin_family - AF_INET; // sin6_family AF_INET6
    // 服务器端口号
    servaddr.sin_port = htons(PORT); // htons主机到网络短整数函数 sin6_port
    // 服务器IP地址
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) { // inet_pton呈现形式到数值 sin6_addr
        err_quit("inet_pton error");
    }
    // sockaddr通用套接字地址结构
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
        err_quit("connect error");
    }
    // TCP是一个没有记录边界的字节流协议,也不提供记录结束标志
    // 从服务器返回的数据有任意种分节方式
    // read返回0表明对端关闭连接,返回负值表明发生错误
    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
    }
    // Unix在一个进程终止时总是关闭该进程所有打开的描述符
    exit(0);
}