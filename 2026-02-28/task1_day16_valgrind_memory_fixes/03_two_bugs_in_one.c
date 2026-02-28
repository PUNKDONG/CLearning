/*
Day16 练习3：一个程序修两类内存问题（综合）

目标：
- 在同一个程序里修复至少 2 类内存问题。

要求：
1. 自己写一个中等规模程序（建议 3 个函数以上）。
2. 程序中故意包含至少 2 类问题，例如：
   - 泄漏（malloc 后未 free）
   - 非法访问（越界或 use-after-free）
3. 用 valgrind 记录修复前日志。
4. 逐个修复后再次跑 valgrind，记录修复后日志。

建议命令：
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
  --log-file=vg_03_before.log ./app

验收：
1. 修复前日志里至少有 2 类问题。
2. 修复后 `ERROR SUMMARY` 明显下降（理想是 0）。
3. 你能说明每个修复的理由（不是只改到不报错）。
*/

