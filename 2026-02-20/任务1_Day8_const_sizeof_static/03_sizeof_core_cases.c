/*
Day8 练习3：sizeof 关键场景

目标：彻底搞清 sizeof 在数组和指针上的行为差异。

要求：
1. 在 main 中定义：
   - int a[10]
   - int *p = a
2. 分别打印以下表达式结果（用 %zu）：
   - sizeof(a)
   - sizeof(a[0])
   - sizeof(p)
   - sizeof(*p)
   - sizeof(a)/sizeof(a[0])
3. 写一个函数，参数是 int arr[] 或 int *arr，
   在函数里打印 sizeof(arr)，并和 main 里的 sizeof(a) 对比。
4. 用注释解释：
   - 为什么 main 中能算出数组长度
   - 为什么函数参数里算不出来

验收标准：
- 解释中必须出现“数组退化为指针”这句话。
- 输出格式不出现类型 warning。
*/
#include<stdio.h>
void my_print(int *arr){
   printf("%zu\n",sizeof(arr));
   //这个肯定是8,因为这个arr是地址,而main中的是40因为是整个数组的长度,数组传参数的时候退化成指针了
}
int main(void){
   int a[10]={1,2,3,4,5,6,7};
   int *p=a;
   printf("%zu %zu %zu %zu %zu\n",sizeof(a),sizeof(a[0]),sizeof(p),sizeof(*p),sizeof(a)/sizeof(a[0]));
   my_print(a);
   return 0;
}