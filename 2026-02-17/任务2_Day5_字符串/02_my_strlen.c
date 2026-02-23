/*
Day5 题目2：手写 strlen

要求：
1. 自己实现函数：
   size_t my_strlen(const char *s);
2. 功能与 strlen 一致：返回字符串长度（不包含 '\0'）。
3. 在 main 中准备至少 4 组测试：
   - 普通英文串（如 "abc"）
   - 空字符串（""）
   - 含空格字符串（如 "a b c"）
   - 含中文 UTF-8 字符串（观察结果，不要求按“中文字符个数”计数）
4. 打印 my_strlen 与库函数 strlen 的结果做对比。

提示：
- 循环直到遇到 '\0'。
- 返回类型建议用 size_t。
*/
#include<stdio.h>
#include <string.h>

size_t my_strlen(const char *s){
   size_t len=0;
   while(s[len]!='\0'){
      len++;
   }
  return len;
}
int main(void){
   char a[]="abc";
   char b[]="";
   char c[]="a b c";
   char d[]="中国";
   printf("a mylen is %zu,strlen is %zu\n",my_strlen(a),strlen(a));
    printf("b mylen is %zu,strlen is %zu\n",my_strlen(b),strlen(b));
 printf("c mylen is %zu,strlen is %zu\n",my_strlen(c),strlen(c));
 printf("d mylen is %zu,strlen is %zu\n",my_strlen(d),strlen(d));
return 0;
}

