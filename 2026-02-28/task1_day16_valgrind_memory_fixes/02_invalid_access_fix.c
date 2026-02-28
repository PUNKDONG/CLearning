/*
Day16 练习2：非法访问定位与修复（进阶）

目标：
- 用 valgrind 定位并修复 `Invalid read/write`。

要求：
1. 自己写一个会发生非法访问的小程序（例如：
   - 数组越界
   - free 后继续访问
   二选一即可）。
2. 跑 valgrind 并记录报错调用栈（修复前日志）。
3. 修复后再次跑 valgrind，确认对应错误消失。

验收：
1. 修复前出现 `Invalid read` 或 `Invalid write`。
2. 修复后该类错误消失。
3. 能指出具体错误行和触发原因。
*/

