/*
Day9 题1：学生结构体基础

要求：
1. 定义 `struct Student`，至少包含：id、name、age、score。
2. 在 main 中创建 3 个学生变量并赋值。
3. 打印每个学生完整信息。
4. 计算并打印：
   - 平均分
   - 最高分学生的姓名

验收：
- 输出字段完整。
- 最高分逻辑正确。
*/

#include <stdio.h>
struct student
{
   int id;
   char *name;
   int age;
   int score;
};
void printMessage(struct student *s)
{
   printf("id %d,name %s,age %d,score %d\n", s->id, s->name, s->age, s->score);
}
void printAVGandName(struct student arr[], size_t n)
{
   int sum = 0;
   char*maxname=arr[0].name;
   int max=arr[0].score;
   for (size_t i = 0; i < n; i++)
   {  if(arr[i].score>max){
      max=arr[i].score;
      maxname=arr[0].name;
   }
      sum = sum + arr[i].score;
   }
   printf("avg is %ld\n", sum / n);
 printf("max name is %s\n",maxname);
}
int main(void)
{
   struct student A = {
       .id = 1,
       .name = "A",
       .age = 20,
       .score = 33};
   struct student B = {
       .id = 2,
       .name = "B",
       .age = 20,
       .score = 96};
   struct student C = {
       .id = 3,
       .name = "C",
       .age = 20,
       .score = 88};
   printMessage(&A);
   printMessage(&B);
   printMessage(&C);
   struct student arr[] = {A, B, C};
   printAVGandName(arr,sizeof(arr)/sizeof(arr[0]));
   return 0;
}