/*
Day7 练习3：指针窗口遍历（偏移区间）

要求：
1. 定义 int 数组 arr（至少 10 个元素）。
2. 实现函数：
   void print_window(const int *arr, size_t n, size_t start, size_t count);
3. 功能：
   - 从 start 开始，连续打印 count 个元素
   - 只允许使用指针偏移，不允许写 arr[i]
   - 处理越界场景（例如 start>=n、start+count>n）
4. 在 main 中测试：
   - 正常窗口
   - 部分越界窗口
   - 完全越界窗口

提示：
- 这个练习是“地址偏移 + 边界保护”的组合训练。
*/

#include<stdio.h>
void print_window(const int *arr, size_t n, size_t start, size_t count){
   if(start>=n){
      printf("完全越界");
   }else if(start+count>n){
         printf("部分越界");
         for(size_t i=start;i<n;i++){
            printf("%d ",*(arr+i));
         }
      }
      else{
          for(size_t i=start;i<start+count;i++){
            printf("%d ",*(arr+i));
         }
      }
   }

int main(void){
   int arr[]={1,2,3,4,5,6,7,8,9,10};
   size_t n = sizeof(arr) / sizeof(arr[0]);

   printf("正常窗口(start=2,count=4): ");
   print_window(arr, n, 2, 4);
   printf("\n");

   printf("部分越界窗口(start=8,count=5): ");
   print_window(arr, n, 8, 5);
   printf("\n");

   printf("完全越界窗口(start=12,count=3): ");
   print_window(arr, n, 12, 3);
   printf("\n");

   return 0;
}
