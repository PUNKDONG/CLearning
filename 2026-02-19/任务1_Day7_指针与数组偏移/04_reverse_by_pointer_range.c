/*
Day7 练习4：用指针区间反转数组

要求：
1. 定义 int 数组 arr（至少 8 个元素）。
2. 实现函数：
   void reverse_range(int *left, int *right);
3. 功能：
   - 反转 [left, right] 区间（包含端点）
   - 不新建第二个数组
4. 在 main 中分别测试：
   - 反转整个数组
   - 只反转中间一段
5. 反转前后都打印数组，验证结果。

提示：
- 该题核心是“两个指针向中间靠拢并交换”。
- 调用时可传 &arr[l]、&arr[r]。
*/

#include <stdio.h>
void reverse_range(int *left, int *right)
{
   while (left < right)
   {
      int temp = *left;
      *left = *right;
      *right = temp;
      left++;
      right--;
   }
}
void printAll(int *arr, int len)
{
   for (int i = 0; i < len; i++)
   {
      printf("%d ", *(arr + i));
   }
   printf("\n");
}
int main(void)
{
   int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
   int len = sizeof(a) / sizeof(a[0]);
   printAll(a, len);
   reverse_range(&a[0], &a[len - 1]);
   printAll(a, len);
   reverse_range(&a[3], &a[6]);
   printAll(a, len);
   return 0;
}