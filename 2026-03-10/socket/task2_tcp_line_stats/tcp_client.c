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
int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect error");
        close(sockfd);
        return 1;
    };
    char send_buf[BUF_SIZE];
    char recv_buf[BUF_SIZE];
    while (1)
    {
        fgets(send_buf, BUF_SIZE, stdin);
        size_t len = strlen(send_buf);
        if (len > 0 && send_buf[len - 1] == '\n')
        {
            send_buf[len - 1] = '\0';
            len--;
        }
        if (strcmp(send_buf, "exit") == 0)
        {
            break;
        }
        char sendmsg[BUF_SIZE+2];
        snprintf(sendmsg,  sizeof(sendmsg), "%s\n", send_buf);

        send(sockfd, sendmsg, strlen(sendmsg), 0);

        memset(recv_buf, 0, sizeof(recv_buf));
        int index = 0;
        while (1)
        {
            char c;
            ssize_t n = recv(sockfd, &c, 1, 0);
            if (n <= 0)
            {
                printf("服务端已断开连接\n");
                break;
            }
              if (c == '\n') { // 读到换行符，消息结束
                break;
            }
            recv_buf[index++]=c;
        }
        recv_buf[index]='\0';
        printf("收到服务端回复：%s\n", recv_buf);
    }

    return 0;
}