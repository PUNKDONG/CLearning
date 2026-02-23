/*
Day6 练习1：指针基础（* 和 &）

要求：
1. 定义 int 变量 a，初始值自定。
2. 定义 int* 指针 p，让它指向 a。
3. 分别打印：
   - a 的值
   - a 的地址
   - p 的值（地址）
   - *p 的值
4. 通过 *p 修改 a 的值（例如 +10），再打印 a。
5. 再定义一个 int b，让 p 改为指向 b，打印验证 p 和 *p 的变化。

提示：
- 地址打印建议使用 %p，并把参数转为 (void *)。
- 观察“修改 *p 会不会影响原变量”。
*/

#include<stdio.h>
int main(void){
   int a=100;
   int*p=&a;
   printf("a=%d,&a=%p,p=%p,*p=%d\n",a,(void*)&a,(void*)p,*p);
   *p=*p+10;
   printf("a=%d\n",a);
   int b=99;
   p=&b;
   printf("p is %p,*p is %d\n",(void*)p,*p);
   return 0;
}