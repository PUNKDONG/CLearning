/*
题目 5：waitpid 回收子进程

目标：
- 理解为什么父进程要回收子进程
- 理解 `waitpid` 的基本使用

要求：
1. `fork` 创建一个子进程。
2. 子进程打印一句话后退出，并返回一个你指定的退出码，例如 `7`。
3. 父进程使用 `waitpid` 等待这个子进程结束。
4. 父进程打印：
   - 被回收的子进程 PID
   - 子进程是否正常退出
   - 子进程退出码

你需要掌握：
- `waitpid(pid, &status, 0)` 的基本意义
- `WIFEXITED(status)` 判断是否正常退出
- `WEXITSTATUS(status)` 取退出码

边界要求：
- `fork` 和 `waitpid` 都要检查返回值
- 不允许只调用 `waitpid` 不解析 `status`

额外思考：
- `wait` 和 `waitpid` 的区别是什么？
- `waitpid` 第三个参数如果写 `WNOHANG` 会怎样？
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int substatus = 0;
    __pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }
    if (pid == 0)
    {
        printf("sub test\n");
        return 7;
    }
    else
    {
        printf("%d\n", pid);
        int w = waitpid(pid, &substatus, 0);
        if (w == -1)
        {
            perror("waitpid");
            return 1;
        }
         printf("%d\n", WIFEXITED(substatus));
        printf("%d\n", WEXITSTATUS(substatus));
       
    }

    return 0;
}
