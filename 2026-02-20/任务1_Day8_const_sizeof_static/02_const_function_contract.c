/*
Day8 练习2：const 作为函数“只读契约”

目标：理解为什么函数参数经常写 const。

要求：
1. 写一个函数，统计数组元素和：
   - 参数形式必须包含 const（例如 const int *arr, size_t n）
2. 在函数内尝试修改 arr 指向的数据（故意写一行非法代码），观察报错后再注释掉。
3. 在 main 中准备测试数据并调用函数，打印结果。
4. 再写一个“不带 const”的版本，对比两者的风险差异（用注释说明）。

验收标准：
- 你能解释“const 的意义不是性能，而是约束和自文档化”。
- 对同一组数据，函数输出正确。
*/
#include<stdio.h>
int sum(const int* const arr,size_t n){
   int sum=0;
   //int p=11;
   //int *x=&p; 
   for(size_t i=0;i<n;i++){
      if(i==n/2){
         //*(arr+i)=10; 
         //arr[i]=10; 这两种都不合法
       // arr=x;//这样合法了,同时会出错,所以要const int* const arr,size_t n这样参数才对
      }
      sum=sum+*(arr+i);
   } 
   return sum;
}
int error_sum(  int*   arr,size_t n){
   int sum=0;
   int p=11;
   int *x=&p; 
   for(size_t i=0;i<n;i++){
      if(i==n/2){
         *(arr+i)=10; 
         arr[i]=10; 
        arr=x; 
      }
      sum=sum+*(arr+i);
   } 
   return sum;
}
int main(void){
int a[]={1,2,3,4,5,6,7,8,9,10};
printf("%d\n",sum(a,sizeof(a)/sizeof(a[0])));
printf("%d\n",error_sum(a,sizeof(a)/sizeof(a[0])));

}