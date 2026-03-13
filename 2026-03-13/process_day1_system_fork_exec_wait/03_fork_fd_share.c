/*
题目 3：fork 复制文件描述符

目标：
- 理解 `fork` 后父子进程会继承文件描述符
- 理解父子进程对同一个文件的写入现象

要求：
1. 打开一个文件，例如 `fork_fd_output.txt`，要求可写。
2. 调用一次 `fork`。
3. 父进程向文件写入一行：
   - `parent write\n`
4. 子进程向文件写入一行：
   - `child write\n`
5. 程序结束前关闭文件描述符。

你需要掌握：
- `fork` 后父子进程都有这个 fd
- 这个 fd 指向同一个打开文件表项

边界要求：
- `open`、`fork`、`write` 都要检查返回值
- 你要自己观察文件内容的顺序，并思考为什么顺序可能变化

额外思考：
- 为什么父子进程都能写这个文件？
- 为什么不建议依赖写入顺序？
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    int fd=open("io.txt",O_CREAT|O_APPEND|O_WRONLY,0644);
    if(fd==-1){
      perror("open fail");
      return 1;
    }
    char buf[1024];
    __pid_t pid=fork();
    if(pid==0){
      strcpy(buf,"这是子进程写入的\n");
    }else{
      strcpy(buf,"这是父进程写入的\n");   
    }
   ssize_t len= write(fd,buf,strlen(buf));
    if(len==-1){
      perror("write");
      return 1;
    }

    printf("写入成功\n");
    close(fd);
    if(pid==0){
printf("子进程写入成功已经释放\n");
    }else{
printf("父进程写入成功已经释放\n");
    }

    return 0;
}
