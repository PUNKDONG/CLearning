/*
Day15 练习1：断点 + 单步 + 变量观察（基础）

目标：
- 学会 break / run / next / step / print / info locals 的基本组合。

任务要求：
1. 自己写一个小程序，至少包含 2 个函数：
   - 一个函数做简单计算（例如累加、平均值、最大值）。
   - main 调用该函数并输出结果。
2. 在程序中故意放一个“结果不符合预期”的小逻辑（不要语法错误）。
3. 用 gdb 定位问题：
   - 在 main 下断点并 run
   - 用 next/step 跑到问题处
   - 用 print 或 info locals 查看关键变量
4. 修复后再次运行，确认结果正确。

最少调试命令清单（要实际执行）：
- break main
- run
- next
- step
- print 变量名
- info locals
- continue

验收：
- 你能指出“哪一行逻辑错了”并说出证据（变量值）。
*/

#include <stdio.h>
#include <stdlib.h>
int sum(int a, int b)
{
   return a + b;
}
int main(void)
{
   int a = 1;
   int b = 2;
   printf("%d\n", sum(a, b));
   return 0;
}