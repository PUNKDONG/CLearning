/*
 * TODO:
 * 实现 TCP 客户端。
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
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024
int main(void){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_addr={0};
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(SERVER_IP);
    server_addr.sin_port=htons(PORT);

    if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
        perror("connect error");
        close(sockfd);
        return 1;
    };
    char send_buf[BUF_SIZE];
    char recv_buf[BUF_SIZE];
    while (1)
    {
      scanf("%s",send_buf);
      send(sockfd,send_buf,strlen(send_buf),0);

      memset(recv_buf,0,sizeof(recv_buf));

     ssize_t recv_len = recv(sockfd, recv_buf, BUF_SIZE-1, 0);
        if (recv_len <= 0) {
            printf("服务端已断开连接\n");
            break;
        }
        printf("收到服务端回复：%s\n", recv_buf);
    }



return 0;
}