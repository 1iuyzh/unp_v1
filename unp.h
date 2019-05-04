#define SA struct sockaddr

/* unix */
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

/* unp */
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
 * POSIX信号处理 处理SIGCHLD信号 wait和waitpid函数
 * accept前连接终止 服务器进程终止 SIGPIPE信号 服务器主机崩溃 服务器主机崩溃后重启 服务器主机关闭 */