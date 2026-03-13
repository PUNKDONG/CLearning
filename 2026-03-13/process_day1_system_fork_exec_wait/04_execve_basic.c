/*
题目 4：execve 基础案例

目标：
- 理解 `execve` 会用新程序替换当前进程映像
- 理解 `execve` 成功后不会返回

要求：
1. 先 `fork` 创建一个子进程。
2. 子进程中调用 `execve` 执行 `/bin/ls`。
3. 让 `ls` 至少带一个参数，例如 `-l`。
4. 如果 `execve` 失败，要 `perror("execve")`。
5. 父进程打印一句话，例如 `parent waiting child...`

你需要掌握：
- `execve` 成功后不会继续执行后面的代码
- `execve` 失败才会返回 `-1`

边界要求：
- 你需要自己正确构造 `argv`
- 第一个参数通常是程序路径，例如 `/bin/ls`
- `argv` 最后一个元素必须是 `NULL`
- `envp` 可以先传 `NULL`

额外思考：
- 为什么 `fork` 常常和 `execve` 配合使用？
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    __pid_t pid=fork();
    if(pid==0){
        char *env[]={NULL};
        char *path="/bin/ls";
        char *argv[]={"/bin/ls","-l",NULL};
        int re=execve(path,argv,env);
        if(re==-1){
            perror("execve error");
            return -1;
        }
    }else{
        printf("parent waiting child...\n");
    }
    return 0;
}
