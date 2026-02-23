/*
题目1：函数拆分 + 作用域基础

目标：
1. 练习把逻辑拆成多个函数。
2. 理解局部变量、全局变量、static 局部变量的区别。

要求：
1. 定义一个全局变量 g_total_calls，初始值为 0。
2. 写一个函数：
   int add_and_count(int a, int b);
   - 返回 a + b
   - 每调用一次，g_total_calls + 1
3. 写一个函数：
   void print_local_counter(void);
   - 函数内部使用 static int local_counter
   - 每次调用 local_counter 自增并打印
4. 在 main 中：
   - 至少调用 add_and_count 3 次，并打印每次结果
   - 每次调用后打印 g_total_calls
   - 调用 print_local_counter 3 次，观察输出变化
5. 再在 main 中定义一个与全局变量同名的局部变量（例如 g_total_calls），
   打印它和真正全局变量（可通过函数间接打印）的值，体会“作用域遮蔽”。

提示：
- 你可以再写一个函数 void print_global_calls(void) 专门打印真正的全局计数。
- 不要用指针，先把函数和作用域概念跑通。
*/
#include<stdio.h>
int add_and_count(int a, int b);
void print_local_counter(void);

int  g_total_calls=0;
int main(void){
   int a=0;int b=0;
   int  g_total_calls=999;
   for(int i=0;i<3;i++){
      printf("%d\n",add_and_count(a,b));
      a++;
      b++;
      printf("%d\n",g_total_calls);
   }
      for(int i=0;i<3;i++){
      print_local_counter();
   }
   return 0;
}
int add_and_count(int a, int b){
   g_total_calls++;
   return a+b;
}
void print_local_counter(void){
  static int local_counter = 0;  
  local_counter++;
  printf("%d\n",local_counter);
}