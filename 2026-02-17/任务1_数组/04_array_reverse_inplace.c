/*
Day4 题目4：数组反转（原地）

要求：
1. 定义一个 int 数组（至少 8 个元素）。
2. 编写逻辑将数组“原地反转”（不新建第二个数组）。
3. 反转前后都打印完整数组，便于对比结果。

提示：
- 使用双指针/双下标思路：
  left 从 0 开始，right 从 length-1 开始，向中间移动并交换。
*/
#include<stdio.h>
void reverse(int* a,int len){
  int left=0;int right=len-1;
  while(left<right){
    int temp=a[left];
    a[left]=a[right];
    a[right]=temp;
    left++;
    right--;
  }

}
int main(void){
  int a[8]={1,2,3,4,5,6,7,8};
int len=sizeof(a)/sizeof(a[0]);
  for(int i=0;i<len;i++){
    printf("%d\n",a[i]);
  }
reverse(a,len);

    for(int i=0;i<len;i++){
    printf("%d\n",a[i]);
  }
  return 0;
}