/*
Day14 练习3（挑战）：统计文本文件单词总数

题目目标：
- 统计文本文件中的单词总数。

建议规则：
- 以空白字符（空格/\t/\n）分隔单词。
- 连续空白只算分隔，不产生空单词。

要求：
1. 使用 fopen + fgets（或 fgetc）读取。
2. 统计逻辑清晰，不漏计、不重计。
3. 文件打开失败、读取失败都要 perror。
4. 输出总单词数。

边界条件：
1. 空文件（结果应为 0）。
2. 连续多个空白分隔。
3. 文件不存在时要有明确报错。

验收标准：
1. 统计结果合理且可复现。
2. 任意 I/O 错误都能输出明确错误信息。
3. 编译无 warning（-std=c11 -Wall -Wextra）。
*/
#include <stdio.h>
#include <stdlib.h> // 用于 exit()
 
int main(int argc, char *argv[])
{ if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("fopen");
        return -1;
    }
    int count = 0;
    char buf[1024];
    int inword = 0;
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {   
        for (int i = 0; buf[i] != '\0'; i++)
        {   int is_word = ((buf[i] >= 'A' && buf[i] <= 'Z') ||
                    (buf[i] >= 'a' && buf[i] <= 'z') ||
                    (buf[i] == '_'));
            if (is_word)
            {
               if(inword == 0){
                count++;
                inword = 1;
               }
            }else{
                inword = 0;
            }
        } 
    }
    if (ferror(fp)) {
        perror("fgets");
        fclose(fp);
        return -1;
    }
    printf("%d\n", count);
    if (fclose(fp) == EOF) {
        perror("fclose");
        return -1;
    }
    return 0;
    
}
