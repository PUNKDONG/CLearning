/*
Day8 练习5：综合题（const + sizeof + static）

目标：把三个知识点组合到一个小功能里。

需求背景：
你在做一个数据采样模块，每次传入一个整型数组，
模块只读数据，计算平均值，并记录“调用总次数”。

要求：
1. 写函数处理数组：
   - 形参必须使用 const（保证只读）
2. main 中使用 sizeof 计算数组长度后再传入函数。
3. 函数内部用 static 变量记录累计调用次数，每次调用都打印次数。
4. 至少测试 3 组数组：
   - 正常数组
   - 全 0 数组
   - 单元素数组
5. 对非法输入做保护（如空指针、长度为 0）。

验收标准：
- 无越界、无 warning。
- 输出中清晰看到“第几次调用”和每组计算结果。
*/
#include<stdio.h>
int sum(const int *a,size_t n){
   if(a==NULL)return 0;
   if(n==0)return 0;
   static int count=0;
   count++;
   printf("count:%d\n",count);
   int sum=0;
   for(size_t i=0;i<n;i++){
      sum=sum+*(a+i);
   }

   return sum/n;
}
int main(void){
int a[]={1,2,3,4,5,6,7};
int b[]={0,0,0,0,0,0,0};
int c[]={1};
printf("%d %d %d\n",sum(a,sizeof(a)/sizeof(a[0])),sum(b,sizeof(b)/sizeof(b[0])),sum(c,sizeof(c)/sizeof(c[0])));
int* d=NULL;
printf("%d %d\n",sum(d,0),sum(b,0));

return 0;
}