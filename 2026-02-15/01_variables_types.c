/*
题目1：变量与类型基础

要求：
1. 定义并初始化以下变量：
   - int 类型：年龄（age）
   - float 类型：身高（height）
   - char 类型：等级（grade）
2. 使用 printf 分别输出这 3 个变量的值。
3. 再定义一个 int 变量 year，表示出生年份。
4. 通过当前年份 - year 计算年龄，并输出结果。

提示：
- 注意不同类型在 printf 中使用不同的格式化占位符。
- 先不考虑非法输入，先把基础语法写通。
*/

#include<stdio.h>
int main(void){
   int age=25;
   float height=175;
   char grade='A';
   int year=2001;
   int now=2026;
   printf("age is %d\n",age);
   printf("height is %f\n",height);
   printf("grade is %c\n",grade);
   printf("age is %d\n",now-year);
   return 0;
}