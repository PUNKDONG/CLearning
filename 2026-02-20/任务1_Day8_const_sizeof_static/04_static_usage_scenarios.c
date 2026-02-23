/*
Day8 练习4：static 使用场景

目标：掌握 static 在“函数内”和“文件作用域”的两种语义。

要求：
1. 写一个函数，每次调用都打印调用次数（使用 static 局部变量）。
2. main 中连续调用该函数 5 次，验证计数递增且不重置。
3. 定义一个文件作用域 static 变量作为模块内部状态，
   通过两个函数读写它并打印。
4. 再定义一个 `static` 函数 `helper(const char *tag)`：
   - 功能：统一打印当前模块内部状态（例如 `a` 的值）
   - 要求：在 `printA1` 和 `printA2` 内部都调用一次 `helper`
   - 目的：演示“文件内私有函数”的用法（仅本文件可见）

验收标准：
- 你能解释“作用域”和“生命周期”的区别。
//作用域的意思就是这个变量在什么作用域里面有效,生命周期就是什么周期里面有效不被销毁
- 程序输出能证明 static 局部变量只初始化一次。
*/

#include<stdio.h>
static int a=0;
static void helper(const char* tag){
    printf( "%s print %d\n",tag,a);
}
void printA1(){
   a=a+10;
   helper("A1");
}

void printA2(){
   a=a+5;
   helper("A2");
}
void staticlocal(){
   static int num=0;
   num++;
   printf("%d ",num);
}

int main(void){
   for(int i=0;i<5;i++){
      staticlocal();
   }
   printf("\n");
   printA1();
   printA2();
return 0;
}
