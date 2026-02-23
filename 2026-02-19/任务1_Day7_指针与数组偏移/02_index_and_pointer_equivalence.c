/*
Day7 练习2：下标与指针等价练习

要求：
1. 定义 int 数组 arr（至少 8 个元素）。
2. 分别用两种方式求和并输出(这题不做,太简单了浪费时间)：
   - 下标方式：arr[i]
   - 指针方式：*(arr + i)
3. 再实现一个函数：
   int get_value_by_ptr(const int *arr, size_t n, size_t idx);
   - 使用指针偏移返回第 idx 个元素
   - 若 idx 越界，返回一个约定值（例如 -1）
4. 在 main 中测试 idx=0、idx=中间值、idx=越界 的场景。

提示：
- 这个练习目标是强化 arr[i] <=> *(arr+i) 的等价关系。
*/
#include<stdio.h>
int get_value_by_ptr(const int *arr, size_t n, size_t idx){
   if(idx<=0||idx>n){
      return -1;
   }else{
      return *(arr+idx-1);
   }
}
 
int main(void){
int arr[]={1,2,3,4,5,6,7,8};
  printf("%d\n",get_value_by_ptr(arr,sizeof(arr)/sizeof(arr[0]),1));
 
  printf("%d\n",get_value_by_ptr(arr,sizeof(arr)/sizeof(arr[0]),4));

  printf("%d\n",get_value_by_ptr(arr,sizeof(arr)/sizeof(arr[0]),99));
return 0;
}