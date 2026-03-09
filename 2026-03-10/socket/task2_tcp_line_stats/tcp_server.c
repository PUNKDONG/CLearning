/*
 * TODO:
 * 实现 TCP 服务端。
 * 题目见 task2_tcp_line_stats.md
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8888
 #define BUF_SIZE 1024
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    socklen_t socklen = sizeof(server_addr);
    if (bind(sockfd, (struct sockaddr *)&server_addr, socklen) == -1)
    {
        perror("bind error");
        close(sockfd);
        return 1;
    }
    if (listen(sockfd, 5) == -1)
    {
        perror("listen erro");
        close(sockfd);
        return 1;
    }
    printf("TCP listen start\n");
    char recv_buf[1024];

    while (1)
    {
        struct sockaddr_in clientaddr = {0};
        socklen_t clientlen = sizeof(clientaddr);
        int connectfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientlen);
        if (connectfd == -1)
        {
            perror("accept error");
            continue;
        }

        while (1) {
            int index = 0;
            memset(recv_buf, 0, sizeof(recv_buf));
            char c;
            while (1) {
                ssize_t recv_len = recv(connectfd, &c, 1, 0);
                if (recv_len <= 0) {
                    printf("客户端 %s:%d 已断开连接\n",
                           inet_ntoa(clientaddr.sin_addr),
                           ntohs(clientaddr.sin_port));
                    close(connectfd);
                    connectfd = -1;
                    break;
                }
                if (c == '\n') {
                    break;
                }
                recv_buf[index++] = c;
            }
            if (connectfd == -1) {
                break;
            }

            recv_buf[index] = '\0';
            printf("收到客户端 %s:%d 数据 %s\n",
                   inet_ntoa(clientaddr.sin_addr),
                   ntohs(clientaddr.sin_port),
                   recv_buf);
            recv_buf[index] = '\n';
            send(connectfd, recv_buf, index + 1, 0);
        }
    }

    close(sockfd);
    return 0;
}
