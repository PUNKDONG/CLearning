/*
 * TODO:
 * 实现 UDP 服务端。
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
#define BUF_SIZE 1024
 
int main(void){
    int socketfd=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in server_addr={0};
    server_addr.sin_port= htons(PORT);
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_family=AF_INET;
    if(bind(socketfd,(struct sockaddr*)(&server_addr),sizeof(server_addr))==-1){
        perror("bind failed");
        close(socketfd);
        exit(1);
    }
    //以上就是绑定成功了,然后就是这个循环接受
    char buf[2048];
    struct sockaddr_in client_addr={0};
    socklen_t client_addr_len=sizeof(client_addr);
    while(1){
        memset(buf,0,sizeof(buf));
        int recev=recvfrom(socketfd,buf,sizeof(buf)-1,0,(struct sockaddr*)&client_addr,&client_addr_len);
        if(recev==-1){
            perror("receive erro");
            continue;
        }
        printf("%s :%d %s recv_len=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),buf,client_addr_len);
        
    }

    return 0;
}