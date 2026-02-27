/*
Day15 练习2：崩溃定位（backtrace）

目标：
- 程序崩溃时，学会用 bt 找到具体调用链和崩溃位置。

任务要求：
1. 自己写一个会崩溃的程序（例如空指针解引用、越界访问）。
2. 用 gdb 运行它并复现崩溃。
3. 在崩溃后执行：
   - bt
   - frame 0
   - list
   - print 关键变量
4. 找出导致崩溃的根因并修复，再验证不崩溃。

最少调试命令清单（要实际执行）：
- run
- bt
- frame 0
- list
- print 变量名

验收：
- 你能说清楚“崩溃发生在哪个函数、哪一行、为什么”。
*/

#include <stdio.h>

static int compute_score(int *p, int base)
{
    /* 故意崩溃点：p 为空时解引用 */
    *p = base + 10;
    return *p;
}

static int process_user(int uid)
{
    int *score_ptr = NULL; /* 故意传空指针，触发崩溃 */
    return compute_score(score_ptr, uid);
}

int main(void)
{
    int uid = 42;
    int result = process_user(uid);
    printf("result=%d\n", result);
    return 0;
}
