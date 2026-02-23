/*
Day4 题目2：数组遍历与统计

要求：
1. 定义一个 int 数组，至少 10 个元素（可含正数、负数、0）。
2. 使用 for 循环遍历数组，分别统计并输出：
   - 所有元素之和
   - 最大值
   - 最小值
   - 偶数个数
3. 输出每个元素及其下标，格式示例：
   index=0, value=...

提示：
- 最大/最小值可先用第 0 个元素初始化。
*/
#include<stdio.h>
#include<limits.h>
int main(void){
   int a[10]={-2,-1,0,1,2,3,4,5,6,7};
   int max=INT_MIN;
   int min=INT_MAX;
   int sum=0;
   int count=0;
   for(int i=0;i<10;i++){
      sum=sum+a[i];
      if(a[i]>max){
         max=a[i];
      }
      if(a[i]<min){
         min=a[i];
      }
      if(a[i]%2==0){
         count++;
      }
      printf("index=%d,value=%d\n",i,a[i]);
   }
   printf("max is %d,min %d,sum %d,count %d\n",max,min,sum,count);
   return 0;
}