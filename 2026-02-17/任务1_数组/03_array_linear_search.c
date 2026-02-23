/*
Day4 题目3：线性查找（顺序查找）

要求：
1. 定义一个 int 数组（元素自定）。
2. 从标准输入读取一个目标值 target。
3. 在数组中查找 target：
   - 找到则输出其下标
   - 找不到输出 -1
4. 如果目标值出现多次，输出第一次出现的位置。

进阶（可选）：
1. 输出目标值出现的总次数。
*/
#include<stdio.h>
int find(int a[],int len,int target){
   for(int i=0;i<len;i++){
      if(a[i]==target){
         return i;
      }
   }
   return -1;
}
int main(void){
   int a[5]={1,2,3,4,5};
   int index=0;
   scanf("%d",&index);
   if(index<0||index>=(sizeof(a)/sizeof(a[0]))){
      return 0;
   }
   printf("%d\n",find(a,5,index));
   return 0;
}
