/*
Day10 练习3：动态数组插入/删除（不扩容）

在已有的 IntVec 结构体基础上，实现插入与删除。

要求：
1. 实现函数：
   - int intvec_insert(struct IntVec *v, size_t index, int value);
     允许 index == size（等价于 push）。
   - int intvec_erase(struct IntVec *v, size_t index);

2. 插入：
   - 若 size == cap，返回 -1
   - index > size 返回 -1
   - 否则从尾部向后移动元素，插入 value，size++

3. 删除：
   - index >= size 返回 -1
   - 否则从 index 后移元素覆盖，size--

4. 在 main 中测试：
   - 初始数组 {1,2,3,4}
   - 在 index 0 插入 99
   - 在 index 2 插入 88
   - 删除 index 1
   - 打印最终数组

边界条件：
- 空数组删除必须失败。
- index 必须严格校验。
*/
