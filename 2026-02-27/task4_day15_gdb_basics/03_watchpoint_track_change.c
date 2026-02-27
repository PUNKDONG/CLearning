/*
Day15 练习3：watchpoint 追踪变量被改坏（进阶）

目标：
- 学会用 watch 定位“变量在哪里被错误修改”。

任务要求：
1. 自己写一个至少 3 个函数的小程序，包含一个关键变量（例如 sum、state、flag）。
2. 制造一个“变量最终值错误”的问题（程序不一定崩溃）。
3. 用 gdb 调试：
   - 在 main 运行到关键流程后，对该变量下 watch
   - continue 运行，观察在哪个函数/哪一行触发写入
   - 用 bt 和 print 确认这次写入是否合理
4. 修复错误写入逻辑，验证最终结果正确。

最少调试命令清单（要实际执行）：
- break main
- run
- watch 变量名
- continue
- bt
- print 变量名

验收：
- 你能明确指出“哪个函数、哪一行把变量改错了”。
*/

#include <stdio.h>

static void init_state(int *state)
{
    *state = 100;
}

static void apply_bonus(int *state, int bonus)
{
    *state += bonus;
}

static void sanitize_state(int *state)
{
    /*
     * 故意留的逻辑错误：
     * 这个函数本应只做检查，不应修改 state。
     * 你需要用 watch 找到是这里把 state 改坏了。
     */
    if (*state > 0) {
        *state -= 1; /* BUG: 导致最终值错误 */
    }
}

static void print_result(const int *state)
{
    printf("expected=120, actual=%d\n", *state);
}

int main(void)
{
    int state = 0;

    init_state(&state);       /* state = 100 */
    apply_bonus(&state, 20);  /* state = 120 */
    sanitize_state(&state);   /* BUG: state = 119 */

    print_result(&state);
    return 0;
}
