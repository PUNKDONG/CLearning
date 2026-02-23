/*
题目2：if/else 分支练习

要求：
1. 定义一个 int 变量 score（0~100）。
2. 使用 if / else if / else 判断成绩等级并输出：
   - 90~100：A
   - 80~89：B
   - 70~79：C
   - 60~69：D
   - 0~59：E
3. 如果 score 不在 0~100，输出 "invalid score"。

进阶（可选）：
1. 再定义一个 int 变量 attendance（出勤率，0~100）。
2. 只有当 score >= 60 且 attendance >= 75 才输出 "pass"，否则输出 "fail"。
*/

#include<stdio.h>
int main(void){
   int score =0;
   if (scanf("%d", &score) != 1) {
      printf("input error\n");
      return 1;
   }
   if(score<=100&&score>=90){
      printf("A");
   }else if(score<=89&&score>=80){
      printf("B");
   }else if(score<=79&&score>=70){
      printf("C");
   }else if (score<=69&&score>=60){
      printf("D");
   }else if (score>=0&&score<=59){
      printf("E");
   }else{
      printf("invalid score");
      return 1;
   }
   int attendance=0;
   printf("\n"); 
   if (scanf("%d", &attendance) != 1) {
      printf("input error\n");
      return 1;
   }
   if(score>=60&&attendance>=75){
      printf("pass");
   }else{
      printf("fail");
   }
return 0;
}
