#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "args.h"
#include "grep.h"
 

void grep_file(Arg args)
{
    int i_flag = args.i_flag;
    int n_flag = args.n_flag;

    const char *word = args.word;

    const char *filepath = args.filepath;
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return;
    }
    char buf[256];
    int line_number = 1;
  
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        // #TODO 这里传入的是 const char *word；如果 grep.h 仍是 char * 形参会触发 const 丢弃警告。
        if (match_line(buf, word, i_flag))
        {
            if(n_flag)
            {
                printf("%d :", line_number);
            }
            printf("%s", buf);
        }

        line_number++;
    }
    fclose(fp);
    return;
}
int match_line(const char *buf, const char*word, int i_flag)
{
    if (buf == NULL || word == NULL)
    {
        return 0;
    }

    if (word[0] == '\0')
    {
        return 1;
    }

    if (!i_flag)
    {
        return strstr(buf, word) != NULL;
    }

    for (size_t i = 0; buf[i] != '\0'; i++)
    {
        size_t j = 0;
        while (word[j] != '\0' && buf[i + j] != '\0')
        {
            unsigned char c1 = (unsigned char)buf[i + j];
            unsigned char c2 = (unsigned char)word[j];
            if (tolower(c1) != tolower(c2))
                break;
            j++;
        }
        if (word[j] == '\0')
            return 1; // 全部匹配
    }
    return 0;
}
