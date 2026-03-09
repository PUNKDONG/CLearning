/*
 * TODO:
 * 实现 UDP 客户端。
 * 题目见 task1_udp_ping_time.md
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h> 
#include<string.h>
#include<arpa/inet.h>
#include <unistd.h>
#define PORT 8888
#define SERVERADDR "127.0.0.1"
int main(void){
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1){
        perror("init erro"); 
        close(sockfd);
        return 1;
    }
   struct sockaddr_in serveraddr={0};
   serveraddr.sin_family=AF_INET;
   serveraddr.sin_port=htons(PORT);
   serveraddr.sin_addr.s_addr=inet_addr(SERVERADDR);
   socklen_t slen=sizeof(serveraddr);
    while(1){
        char s[1024];
        scanf("%s",s);
        int send=sendto(sockfd,s,strlen(s),0,(struct sockaddr*)&serveraddr,slen);
        if(send==-1){
            printf("send error\n");
            continue;
        }
        printf("success send:%s",s);
    }
    return 0;
}