/*
Day6 练习2：实现 swap(int *a, int *b)

要求：
1. 自己实现函数：
   void swap(int *a, int *b);
2. 在 main 中定义两个 int 变量 x、y，调用 swap(&x, &y) 前后打印结果。
3. 额外测试：
   - x 和 y 初始相等时是否正常
   - 调用 swap(&x, &x) 时程序是否仍能正确运行
4. 在函数中不允许使用全局变量。

提示：
- 你需要一个临时变量 temp。
- 重点理解为什么传的是地址而不是值。
*/
#include<stdio.h>
void swap(int *a,int *b){
   int temp=*a;
   *a=*b;
   *b=temp;
}
int main(void){
   int x=10;
   int y=20;
   swap(&x,&y);
   printf("x=%d,y=%d\n",x,y);
   swap(&x,&x);
   printf("x=%d,y=%d\n",x,y);
   int z=5; 
   int c=5;
   swap(&z,&c);
   printf("z=%d,c=%d\n",z,c);
  
}