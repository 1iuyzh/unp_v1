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

    // 捕获SIGCHLD信号,防止僵死进程
    Signal(SIGCHLD, sig_chld);

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
    pid_t pid;
    for (;;) {
        clien = sizeof(cliaddr);
        // 信号可以由一个进程发给另一个进程,也可以由内核发给某个进程
        // SIGCHLD信号由内核在任一进程终止时发给它的父进程,通过调用sigaction函数设置信号处理函数signal handler
        // POSIX保证被捕获的信号在其信号处理函数运行期间总是阻塞的
        // 如果一个信号在被阻塞期间产生了一次或多次,那么该信号被解阻塞后通常只递交一次
        // 如果一个进程终止时有僵死的子进程,则它的所有僵死子进程的父进程id被重置为1(init进程),由init进程处理僵死进程
        // 在阻塞于accept时捕获到SIGCHLD信号,内核会使accpet返回一个EINTR错误(被中断的系统调用)
        if ((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
            if (errno == EINTR)
                continue;
            else
                err_sys("accept error");
        }
        // fork是Unix中派生新进程的唯一方法
        // 父进程中调用fork之前打开的所有描述符在fork返回之后由子进程分享
        // 存放在硬盘上的可执行文件被Unix执行的唯一方法是由一个现有进程调用exec函数,进程id不变
        if ((pid = Fork()) == 0) {
            Close(listenfd);
            Write(connfd, ...);
            Close(connfd); // 将该套接字标记成已关闭,不能再由调用进程使用,TCP将尝试发送已排队等待发送的任何数据,发送完毕后是正常的TCP连接终止序列
            exit(0); // 进程终止时会关闭所有由内核打开的描述符
        }
        Close(connfd); // 每个文件或套接字都有一个引用计数,引用计数值为0时才会发送FIN
    }

    // 阻塞式I/O,系统调用直到数据报到达且被复制到应用进程的缓冲区中才返回
    // 非阻塞式I/O,当所请求的I/O操作非得把本进程投入睡眠才能完成时,不要投入睡眠而是返回一个错误(需要持续轮询内核)
    // I/O复用,进程阻塞于select或poll,当数据报套接字可读时返回.类似于多线程中使用阻塞式I/O
    // 信号驱动式I/O,让内核在描述符就绪时发送SIGIO信号进行通知,需要开启套接字的信号驱动式I/O功能
    // 异步I/O,信号驱动I/O是内核通知何时可以启动一个I/O操作,异步I/O是内核通知我们I/O操作何时完成
}

// 使用单进程和select的TCP服务器程序
int main(int argc, char **argv) {
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    maxfd = listenfd;
    maxi = -1;
    for (int i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for (;;) {
        rset = allset;
        nready = Select(maxfd + 1; &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
            
            for (int i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }
            if (i == FD_SETSIZE)
                err_quit(...);
            
            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > max)
                max = i;
            if (--nready <= 0)
                continue;
        }

        for (i = 0; i <= max; i++) {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else
                    Writen(sockfd, buf, n);
                if (--nready <= 0)
                    break;
            }
        }
    }
}