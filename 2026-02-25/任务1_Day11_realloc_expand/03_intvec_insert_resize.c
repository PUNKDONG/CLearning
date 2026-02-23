/*
Day11 练习3：insert/erase + 自动扩容与缩容（综合）

要求：
1. 实现：
   - int intvec_insert(struct IntVec *v, size_t index, int value);
   - int intvec_erase(struct IntVec *v, size_t index);
   - int intvec_shrink_if_needed(struct IntVec *v);

2. 规则：
   - insert：index <= size 才允许；size == cap 时先扩容
   - erase：index < size 才允许；删除后判断是否需要缩容
   - shrink：当 size <= cap/4 时，缩容到 cap/2（cap 最小为 1）
   - 所有函数失败返回 -1，成功返回 0
   - 扩/缩容必须使用 realloc 安全写法

3. 在 main 中测试：
   - 初始 cap=2，元素 {1,2}
   - index=0 插入 99
   - index=2 插入 88
   - index=size 插入 77
   - 连续 erase，触发一次缩容

边界条件：
- index > size 必须返回 -1
- 扩/缩容失败时不得改动原数组
- size==0 也允许 shrink（cap 仍至少为 1）
*/
