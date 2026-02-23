/*
Day7 练习5：指针差值与查找位置

要求：
1. 定义 int 数组 arr（元素可重复）。
2. 实现函数：
   int *find_first_ptr(int *arr, size_t n, int target);
   - 找到返回该元素地址
   - 找不到返回 NULL
3. 在 main 中：
   - 调用 find_first_ptr 查找 target
   - 若找到，打印：
     1) 元素值
     2) 元素地址
     3) 下标（用 指针差值 计算：ptr - arr）
   - 若找不到，打印 not found
4. 至少测试 2 个值：一个存在，一个不存在。

提示：
- ptr - arr 的结果是“元素个数差”，不是字节差。
- 这个练习是“查找 + 偏移 + 下标反推”的综合题。
*/

#include <stdio.h>
int *find_first_ptr(int *arr, size_t n, int target)
{
   for (size_t i = 0; i < n; i++)
   {
      if (*(arr + i) == target)
      {
         return arr + i;
      }
   }
   return NULL;
}
int main(void)
{
   int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
   size_t len = sizeof(a) / sizeof(a[0]);
   int *temp = find_first_ptr(a, len, 5);
   if (temp == NULL)
   {
      printf("not found");
   }
   else
   {
      printf("%d %p %ld\n", *temp, (void *)temp, temp - a);
   }
   temp = find_first_ptr(a, len, 99);
    if (temp == NULL)
   {
      printf("not found\n");
   }
   else
   {
      printf("%d %p %ld\n", *temp, (void *)temp, temp - a);
   }
   return 0;
}