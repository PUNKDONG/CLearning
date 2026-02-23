/*
Day6 练习3：指针与数组遍历

要求：
1. 定义一个 int 数组（至少 8 个元素）。
2. 写一个函数，使用“指针方式”遍历数组并打印：
   void print_array_by_ptr(const int *arr, size_t n);
3. 写一个函数，使用“指针方式”计算数组和：
   int sum_by_ptr(const int *arr, size_t n);
4. 在 main 中对比：
   - 下标方式求和
   - 指针方式求和
   两者结果应一致。
5. 进阶（可选）：不允许在函数体中出现 arr[i] 这种写法，只用 *(arr + i) 或移动指针。

提示：
- 这个练习是为后面“指针偏移”和“内存连续”打基础。
*/
#include<stdio.h>
void print_array_by_ptr(const int *arr, size_t n){
   for(size_t i=0;i<n;i++){
      printf("%d\n",*(arr+i));
   }
}
int sum_by_ptr(const int *arr, size_t n){
   int sum=0;
   for(size_t i=0;i<n;i++){
   sum=sum+*(arr+i);
   }
   return sum;
}
int sum_by_index(const int *arr, size_t n){
   int sum=0;
    for(size_t i=0;i<n;i++){
   sum=sum+arr[i];
   }
   return sum;
}
int main(void){
   int a[]={1,2,3,4,5,6,7,8,9}; 
   print_array_by_ptr(a,sizeof(a)/sizeof(a[0]));
   int sum_ptr=sum_by_ptr(a,sizeof(a)/sizeof(a[0]));
   
   int sum_index=sum_by_index(a,sizeof(a)/sizeof(a[0]));
   printf("%d\n",sum_index);
    printf("%d\n",sum_ptr);
   printf("%s\n",sum_index==sum_ptr?"YES":"NO");

   return 0;
}