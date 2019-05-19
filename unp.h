#define SA struct sockaddr

struct in_addr {
    in_addr_t s_addr; // 32位IPv4地址 网络字节序
};

struct sockaddr_in {
    uint8_t sin_len; // 结构体长度
    sa_family_t sin_family; // AF_INET
    in_port_t sin_port; // 16位TCP或UDP端口号 网络字节序
    struct in_addr sin_addr; // 32位IPv4地址 网络字节序
    char sin_zero[8]; // 未使用
};

// 通用套接字地址结构
struct sockaddr {
    uint8_t sa_len;
    sa_family_t sa_family;
    char sa_data[14];
};

// 字节排序函数 Chap.3
uint16_t htons(uint16_t host16bitvalue);
uint32_t htonl(uint32_t host32bitvalue);
uint16_t ntohs(uint16_t net16bitvalue);
uint32_t ntohl(uint32_t net32bitvalue);

// 只要一个Unix函数中有错误发生,全局变量errno就被置为一个指明该错误类型的正值<sys/errno.h>,函数一般返回-1
// err_sys查看errno的值并输出相应的错误消息
void err_sys(char *);
void err_quit(char *);

/* wrapper function */
int Socket(int family, int type, int protocol) {
    int n;
    if ((n = socket(family, type, protocol)) < 0) {
        err_sys("socket error");
    }
    return (n);
}

// SIGCHLD信号处理函数
void sig_chld(int signo) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) // WNOHANG选项告知waitpid在有尚未终止的子进程运行时不要阻塞
        printf(...);
    return;
}

// fdset实现
void FD_ZERO(fd_set *fdset); // clear all bits in fdset
void FD_SET(int fd, fd_set *fdset); // turn on the bit for fd in fdset
void FD_CLR(int fd, fd_set *fdset); // turn off the bit for fd in fdset
int FD_ISSET(int fd, fd_set *fdset); // is the bit for fd on in fdset

// select函数原型,返回已就绪总位数
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, const struct timeval *timeout);

// shutdown不减引用计数就可以激发TCP的正常连接终止序列
// close会终止读和写两个方向的数据传送
// howto=SHUT_RD,关闭连接读一半,套接字中不再有数据可接收,套接字接收缓冲区中的现有数据都被丢弃
// howto=SHUT_WR,关闭连接写一半,半关闭half-close,当前留在套接字发送缓冲区的数据将被发送掉,后跟TCP连接终止序列
// howto=SHUT_RDWR,连接的读半部和写半部都关闭
int shutdown(int sockfd, int howto);

/* 许多网络入侵是由黑客通过发送数据,导致服务器对sprintf的调用使其缓冲区溢出而发生的
 * 必须小心使用的函数还有gets,strcat和strcpy,通常应分别改为调用fgets,strncat和strncpy
 * 更好的替代函数是后来才引入的strlcat和strlcpy,它们确保结果是正确终止的字符串 */

/* Chap.2
 * TCP/UDP/SCTP TCP连接建立和终止 TIME_WAIT状态 缓冲区 */

/* Chap.3
 * 套接字地址结构 value-result参数 字节排序函数 字节操纵函数
 * inet_aton,inet_addr,inet_ntoa函数 inet_pton,inet_ntop函数
 * readn,writen,readline函数 */

/* Chap.4
 * socket函数 connect函数 bind函数 listen函数 accept函数
 * fork和exec函数
 * getsockname和getpeername函数 */

/* Chap.5
 * TCP回射程序
 * 正常启动 正常终止
 * POSIX信号处理 signal函数 SIGCHLD信号 wait和waitpid函数
 * accept前连接终止 服务器进程终止 SIGPIPE信号 服务器主机崩溃 服务器主机崩溃后重启 服务器主机关闭 */

/* Chap.6
 * I/O模型
 * select函数 描述符集就绪条件
 * str_cli函数修订版 批量输入 shutdown函数
 * pselect函数 poll函数 */

/* Chap.16
 * 非阻塞I/O */

/* Chap.26
 * 线程 */