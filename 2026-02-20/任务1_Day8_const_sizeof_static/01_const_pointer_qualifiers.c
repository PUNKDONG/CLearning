/*
Day8 编程题1：const 指针（精简版）

要求（自己写完整程序）：
1. 定义 `int x = 10, y = 20;`
2. `const int *p1`：
   - 先指向 x 打印 `*p1`
   - 再改指向 y 打印 `*p1`
3. `int *const p2`：
   - 固定指向 x
   - 通过 `*p2` 修改 x 两次，每次打印 x、y
4. `const int *const p3`：
   - 固定指向 x
   - 只打印 `*p3` 和地址
5. 把以下非法语句保留为注释，并写错误原因：
   - `*p1 = 30;`
   - `p2 = &y;`
   - `*p3 = 40;`
   - `p3 = &y;`
6. 最后打印 `final x`、`final y`

验收：
- p1 输出应先 10 后 20
- y 始终为 20

编译：
gcc -Wall -Wextra -std=c11 01_const_pointer_qualifiers.c -o p1
*/
#include<stdio.h>
int main(void){
   int x=10;
   int y=20;
   //const的是int* 也就是这个 int const *p1 所以是指向的值不能改,但是地址可以改
   const int *p1=&x;
   printf("%d\n",*p1);
   p1=&y;
   printf("%d\n",*p1);
   //- 通过 `*p2` 修改 x 两次，每次打印 x、y
   //这里是指向一个p1 这个是地址,所以是地址本身不能改变,但是指向的值就可以改变
   int* const p2=&x; 
   *p2=11;
   printf("%d %d\n",x,y);
   *p2=22;
   printf("%d %d\n",x,y);
   const int *const p3=&x;
   printf("%d %p %p\n",*p3,(void*)&x,(void*)p3);
   printf("%d %d\n",x,y);
   //   - `*p1 = 30;` p1是const int *p1.,所以应该是指向的值不能改 int const *p1这样
   // - `p2 = &y;` p2 是int * const p2 ,所以p2本身是不能改变的
   // - `*p3 = 40;` p3怎样都不能变
   // - `p3 = &y;`  p3怎样都不能变

}