/*
题目 2：fork 创建子进程

目标：
- 理解 `fork` 会返回两次
- 理解父子进程如何分支执行

要求：
1. 调用一次 `fork`。
2. 在子进程中打印：
   - 当前进程 PID
   - 父进程 PID
   - 一句固定文本，例如 `I am child`
3. 在父进程中打印：
   - 当前进程 PID
   - 子进程 PID
   - 一句固定文本，例如 `I am parent`
4. `fork` 失败时使用 `perror` 打印错误并退出。

你需要掌握：
- `fork` 返回 `-1`：失败
- `fork` 返回 `0`：当前代码在子进程中
- `fork` 返回 `>0`：当前代码在父进程中，返回值是子进程 PID

边界要求：
- 必须显式区分三种情况：失败 / 子进程 / 父进程
- 不允许只打印一边
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
   
    __pid_t pid=fork();
    if(pid<0){
      printf("创建失败\n");
      return 1;
    }
    if(pid==0){
        printf("自己的pid是%d父亲的pid是%d\n",getpid(),getppid());
        printf("I am child\n");
    }else{
     printf("父亲的pid是%d,儿子pid是%d\n",getpid(),pid);
     printf("I am father\n");
}
    return 0;
}
