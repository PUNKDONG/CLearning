/*
Day7 练习1：地址与偏移观察

要求：
1. 定义 int 数组 arr，至少 6 个元素。
2. 打印以下内容（建议循环）：
   - &arr[i]
   - arr + i
   - *(arr + i)
3. 验证并说明：
   - &arr[i] 和 arr+i 是否相同
   - *(arr+i) 和 arr[i] 是否相同
4. 额外打印相邻元素地址差值，观察步长是否等于 sizeof(int)。

提示：
- 地址建议用 %p 打印，并转换为 (void *)。
- 重点体会“指针加法按元素步长移动”。
*/
#include<stdio.h>
int main(void){
   printf("%zu\n",sizeof(int));
   int arr[]={1,2,3,4,5,6,7};
   for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);i++){
      printf("%p  %p %d\n",(void*)&arr[i],(void*)(arr + i),*(arr + i));
      printf("%s %s\n",(&arr[i])==(arr+i)?"YES":"NO",(*(arr+i))==arr[i]?"YES":"NO");
      if(i>0){
         printf("%p  %p\n",(void*)(arr+i),(void*)(arr+i-1));
      }
   }
}