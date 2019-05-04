#include "unp.h"

// 服务器进程(进程观念)
int main(int argc, char **argv) {
    int listenfd, connfd;
    // 网际套接字地址结构
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    
    // 本地套接字
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // INADDR_ANY服务器进程可以在任意网络接口上接受客户连接
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // 服务器端口被捆绑到所创建的套接字
    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    // 把该套接字转换成一个监听套接字,来自客户的连接可以在该套接字上由内核接受
    Listen(listenfd, LISTENQ); // LISTENQ指定系统内核允许在该监听套接符上排队的最大连接数

    /* 迭代服务器(每个客户都迭代执行一次)iterative server */
    for (;;) {
        // 服务器进程在accept调用中投入睡眠,等待某个客户连接到达并被内核接受
        // 通过三路握手three-way handshake来建立连接
        // 返回一个称为已连接描述符的新描述符,用于与客户通信
        connfd = Accept(listenfd, (SA *) NULL, NULL);
        Write(connfd, buff, strlen(buff));
        // 关闭连接,该调用引发正常的TCP连接终止序列(4个TCP分组) 详见2.6节
        Close(connfd);
    }

    /* 并发服务器concurrent server */
    // fork是Unix中派生新进程的唯一方法
    // 父进程中调用fork之前打开的所有描述符在fork返回之后由子进程分享
    // 存放在硬盘上的可执行文件被Unix执行的唯一方法是由一个现有进程调用exec函数,进程id不变
    pid_t pid;
    for (;;) {
        connfd = Accept(listenfd, ...);
        if ((pid = Fork()) == 0) { // 子进程
            Close(listenfd);
            Write(connfd, ...);
            Close(connfd); // 将该套接字标记成已关闭,不能再由调用进程使用,TCP将尝试发送已排队等待发送的任何数据,发送完毕后是正常的TCP连接终止序列
            exit(0); // 进程终止时会关闭所有由内核打开的描述符
        }
        Close(connfd); // 每个文件或套接字都有一个引用计数,引用计数值为0时才会发送FIN
    }
    // SIGCHLD信号由内核在任一进程终止时发给它的父进程 sigaction函数
    // 在一个信号处理函数运行期间,正被递交的信号是阻塞的
    // 如果一个信号在被阻塞期间产生了一次或多次,那么该信号被解阻塞后通常只递交一次
    // 如果一个进程终止时有僵死的子进程,则它的所有僵死子进程的父进程id被重置为1(init进程),由init进程处理僵死进程

    // 输入操作通常包含两个阶段:
    // 等待数据准备好
    // 从内核向进程复制数据

    // Unix下5种I/O模型:
    // 阻塞式I/O
    // 非阻塞式I/O 调用函数时,如果数据没有准备好,则内核返回一个错误 轮询(polling)
    // I/O复用(select和poll) 进程受阻于select调用,等待可能多个套接字中的任一个变为可读
    // 信号驱动式I/O
    // 异步I/O
}