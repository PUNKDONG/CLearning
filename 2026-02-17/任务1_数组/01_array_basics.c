/*
Day4 题目1：数组基础

要求：
1. 定义一个 int 数组，包含 8 个元素（数值自定）。
2. 打印：
   - 数组第 1 个元素
   - 数组最后 1 个元素
   - 数组长度（不要手写常量，使用 sizeof 计算）
3. 将第 3 个元素修改为 99，再次打印整个数组。

提示：
- 下标从 0 开始。
- 先写死数组，不需要输入。
*/

#include<stdio.h>
void printArray(int a[],int num)
{
   for(int i=0;i<num;i++){
      printf("%d\n",a[i]);
   }

}
int main(void){
   int a[8]={1,2,3,4,5,6,7,8};
   printf("%d\n",a[0]);
   printf("%d\n",a[7]);
   printf("%zu\n",sizeof(a)/sizeof(a[0]));
   a[2]=99;
   printArray(a,8);
return 0;
}