/*
Day14 练习4（挑战）：文件复制器（简化 cp）

题目目标：
- 实现命令行：./app source.txt dest.txt
- 将 source 文件内容复制到 dest 文件。

要求：
1. 输入文件用 "r" 打开，输出文件用 "w" 打开。
2. 循环读写（按行 fgets/fputs 或按块 fread/fwrite）。
3. 每次读写都检查返回值，准确报告失败原因（perror）。
4. 关闭两个文件并检查 fclose 返回值。

边界条件：
1. 源文件不存在。
2. 目标文件不可写（权限不足）。
3. 大文件复制（至少测试几 MB，避免只测小文件）。

验收标准：
1. 复制后源文件和目标文件内容一致。
2. 任意 I/O 错误都能输出明确错误信息。
3. 编译无 warning（-std=c11 -Wall -Wextra）。
*/
#include <stdio.h>
#include <stdlib.h> 
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s source.txt dest.txt\n", argv[0]);
        return 1;
    }
    FILE *source = fopen(argv[1], "r");
    if (source == NULL)
    {
        perror("fopen source");
        return 1;
    }
    FILE *dest = fopen(argv[2], "w");
    if (dest == NULL)
    {
        perror("fopen dest");
        return 1;
    }
    char buf[1024];
   while (fgets(buf, sizeof(buf), source) != NULL) {  
    if (fputs(buf, dest) == EOF) {
        perror("fputs");
        return 1;
    }
   }
   if (fclose(source) == EOF) {
        perror("fclose source");
        return 1;
    }
    if (fclose(dest) == EOF) {
        perror("fclose dest");
        return 1;
    }
    return 0;

}