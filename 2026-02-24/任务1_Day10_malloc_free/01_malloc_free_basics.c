/*
Day10 练习1：malloc/free 基础

要求：
1. 从标准输入读取一个整数 n (n > 0)。
2. 使用 malloc 在堆上分配一个 int 数组，长度为 n。
3. 用 for 循环给数组赋值：a[i] = i * 2。
4. 计算数组元素总和并打印。
5. free 释放内存，指针置为 NULL。

边界条件：
- 如果 malloc 返回 NULL，打印错误并直接退出。
- 读入失败时，打印错误并退出。

提示：
- malloc 的单位是字节：n * sizeof(int)。

补充要求（覆盖常见坑）：
在代码里写出以下“错误写法”的注释示例，并说明错因：
- 泄漏：malloc 后直接 return
- double free：对同一指针 free 两次
- use-after-free：free 后继续访问
*/
