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

    // 回射程序
    str_cli(stdin, sockfd);

    // Unix在一个进程终止时总是关闭该进程所有打开的描述符
    exit(0);
}

// 使用select的str_cli函数
// 客户阻塞于标准输入期间,如果服务器进程被杀死,客户进程将看不到EOF
// select告知内核在哪些描述符发生可读,可写或有异常时返回
// select返回后使用FD_ISSET测试fd_set数据类型中的描述符,描述符集中任何与未就绪描述符对应的位返回时清0
// select常见编程错误:忘了对最大描述符加1;忘了描述符集是值-结果参数
void str_cli(FILE *fd, int sockfd) {
    int maxfdp1;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
    FD_ZERO(&rset);
    for (;;) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &rset)) { // TCP套接字可读
            if (Readline(sockfd, recvline, MAXLINE) == 0) // 对端TCP发送一个FIN(对端进程终止),read返回0
                err_quit(...);
            // 对端TCP发送一个RST(对端主机崩溃并重启),read返回-1
            Fputs(recvline, stdout); // 对端TCP发送数据,read返回大于0的值
        }
        if (FD_ISSET(fileno(fp), &rset)) { // 标准输入可读
            if (Fgets(sendline, MAXLINE, fp) == NULL)
                return;
            Writen(sockfd, sendline, strlen(sendline));
        }
    }
}

// str_cli再修订版
void str_cli(FILE *fp, int sockfd) {
    int maxfdp1, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n;
    stdineof = 0;
    FD_ZERO(rset);
    for (;;) {
        if (stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1)
                    return;
                else
                    err_quit(...);
            }
            Write(fileno(fp), buf, n);
        }
        if (FD_ISSET(fileno(fp), &rset)) {
            if ((n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            Writen(sockfd, buf, n);
        }
    }
}