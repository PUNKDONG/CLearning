/*
Day11 练习2：push 自动扩容

要求：
1. 继续使用 IntVec：{ int *data; size_t size; size_t cap; }
2. 实现：
   - int intvec_push(struct IntVec *v, int value);
   - 当 size == cap 时，先扩容，再插入。
   - 扩容策略：cap==0 => 1，否则 cap*=2。
   - realloc 失败必须返回 -1，且保持原数据不变。

3. 在 main 中测试：
   - 初始 cap=2
   - push 10 个元素
   - 打印 size/cap 和数组内容

边界条件：
- v==NULL 返回 -1。
- 注意：扩容后要更新 v->data 和 v->cap。
*/
