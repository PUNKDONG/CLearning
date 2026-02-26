/*
Day14 练习1（基础）：逐行读取文本并输出

题目目标：
- 打开一个文本文件，逐行读取并打印到屏幕。
- 处理“文件不存在”或“无法打开”的错误。

要求：
1. 使用 fopen + fgets + fputs(或 printf) + fclose。
2. 打开失败时必须调用 perror 输出原因。
3. 读取循环结束后，用 ferror 判断是否发生读错误。
4. 文件成功打开后，最后必须 fclose，并检查 fclose 返回值。

输入方式建议：
- 命令行参数：./app input.txt
- 如果没有参数，打印用法并退出。

验收标准：
1. 文件存在时，逐行输出内容。
2. 文件不存在时，输出清晰错误（perror）。
3. 编译无 warning（-std=c11 -Wall -Wextra）。
*/
#include <stdio.h>
#include <errno.h>
int main(void)
{
    // FILE *fopen(const char *path, const char *mode);
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
         fclose(fp);
        return 1;
    }
    char buf[256];
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        if(fputs(buf, stdout)==EOF){
            break;
        };
        
    }
    printf("\n");
    if (ferror(fp))
    {
        perror("fgets/fputs failed");
    }

    if (fclose(fp) == EOF)
    {
        perror("fclose failed");
        return 1;
    }
}