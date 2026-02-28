/*
Day16 练习1：内存泄漏定位与修复（基础）

目标：
- 用 valgrind 复现并修复“definitely lost”泄漏。

要求：
1. 自己写一个小程序，至少有 2 次 malloc。
2. 故意漏掉其中一次 free，让 valgrind 报泄漏。
3. 保存修复前日志（如 vg_01_before.log）。
4. 修复后再次跑 valgrind，保存修复后日志（如 vg_01_after.log）。

验收：
1. 修复前存在 `definitely lost`。
2. 修复后 `definitely lost` 变 0。
3. 能解释“哪块内存之前没释放，为什么现在不会泄漏”。
*/

