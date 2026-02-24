/*
Day11 练习1：realloc 安全扩容

要求：
1. 写一个函数：
   int grow_int_array(int **p, size_t *cap);

2. 规则：
   - 将容量扩容为原来的 2 倍（cap==0 时变为 1）。
   - 使用 realloc 的“安全写法”。
   - 成功返回 0，失败返回 -1（原指针不变）。

函数是干嘛的：
- `grow_int_array`：让一个 `int` 数组“安全扩容”，并更新容量。

3. 在 main 中测试：
   - cap=0 -> grow -> cap=1
   - cap=2 -> grow -> cap=4

边界条件：
- 参数为 NULL 直接返回 -1。
- 扩容前检查溢出：cap > SIZE_MAX/2 时返回 -1。
*/
