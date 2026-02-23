/*
Day5 题目1：字符串基础

要求：
1. 定义一个字符数组字符串，例如 "hello_embedded"。
2. 打印：
   - 整个字符串
   - 第 1 个字符
   - 最后 1 个字符（不要手写下标，先求长度再取）
3. 遍历字符串，统计并输出：
   - 字母个数（忽略下划线）
   - 下划线 '_' 的个数
4. 将字符串中的所有小写字母转成大写后输出（可直接在原数组上修改）。

提示：
- C 字符串以 '\0' 结尾。
- 不要使用 strlen 库函数，先自己用循环算长度。
*/
#include<stdio.h>
int main(void){
   char str[]="hello_embedded";
   int len=0;
   while(str[len]!='\0'){
      len++;
   } 
   
   for(int i=0;i<len;i++){
      printf("%c",str[i]);
   }
    printf("\n");
   printf("%c\n",str[0]);
   if(len>=1){
   printf("%c\n",str[len-1]);
   }
     int num=0;
     int linenum=0;
   for(int i=0;i<len;i++){
     if((str[i]>='A'&&str[i]<='Z')||(str[i]>='a'&&str[i]<='z')){
      num++;
     }
     if(str[i]=='_'){
      linenum++;
     }
     if((str[i]>='a'&&str[i]<='z')){
      str[i]=str[i]+'A'-'a';
     }
   }
   printf("word num is %d ,linenum is %d\n",num,linenum);
    for(int i=0;i<len;i++){
      printf("%c",str[i]);
   }
      printf("\n");
   return 0;
}