/*
题目 1：system 基础

目标：
- 理解 `system` 的作用
- 理解返回值的基本含义

要求：
1. 写一个程序，依次执行下面两个命令：
   - `pwd`
   - `ls`
2. 每次调用 `system` 后，打印它的返回值。
3. 如果 `system` 返回 `-1`，使用 `perror` 打印错误。

你需要掌握：
- `system("命令")` 是让 shell 帮你执行命令
- 它适合快速调用系统命令，不适合复杂进程控制

边界要求：
- 不要把两个命令写成一次 `system` 调用
- 每次调用都要检查返回值
*/

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int a= system("pwd");
    printf("%d\n",a);
    if(a==-1){
        perror("error");
    }
    int b=system("ls");
        printf("%d\n",b);
     if(b==-1){
        perror("error");
    }
    return 0;
}
