/*
题目2：头文件基础 + 多文件函数拆分

目标：
1. 学会把函数声明写进 .h，函数实现写进 .c。
2. 学会在 main.c 中 include 头文件并调用函数。

要求（请你自己新建对应文件）：
1. 在同目录新建这 3 个文件：
   - math_utils.h
   - math_utils.c
   - main_split.c
2. 在 math_utils.h 中声明以下函数：
   int max2(int a, int b);
   int min2(int a, int b);
   int abs_diff(int a, int b);
3. 在 math_utils.c 中实现这 3 个函数。
4. 在 main_split.c 中：
   - 读取两个整数
   - 调用以上 3 个函数并打印结果
5. 编译时不要把所有代码写在一个文件里，要用“多文件编译”。

编译目标（你自己完成）：
- 方式1（一步到位）：
  gcc -g -O0 -Wall -Wextra -std=c11 main_split.c math_utils.c -o split_demo
- 方式2（分步）：
  gcc -c main_split.c
  gcc -c math_utils.c
  gcc main_split.o math_utils.o -o split_demo

额外要求（可选）：
- 在头文件里加 include guard（防止重复包含）。
*/
