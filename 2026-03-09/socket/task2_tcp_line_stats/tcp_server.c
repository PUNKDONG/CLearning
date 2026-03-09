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
    struct sockaddr_in clientaddr = {0};
    socklen_t clientlen = sizeof(clientaddr);
    int connectfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientlen);
    if (connectfd == -1)
    {
        perror("connet erro");
        close(sockfd);
        return 1;
    }
    char buf[1024];
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int recv_len = recv(connectfd, buf, sizeof(buf - 1), 0);
          if (recv_len <= 0) { // recv 返回 0 表示客户端断开连接
            printf("客户端 %s:%d 已断开连接\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
            break;
        }
         printf("收到客户端 %s:%d 数据%s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port),buf);
        
        send(connectfd, buf, recv_len, 0);
    }
   
    return 0;
}