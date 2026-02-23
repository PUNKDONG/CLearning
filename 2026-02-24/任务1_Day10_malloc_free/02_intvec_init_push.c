/*
Day10 练习2：动态数组初始化 + push（不扩容）

实现一个固定容量的动态数组（不使用 realloc）。

要求：
1. 定义结构体：
   struct IntVec { int *data; size_t size; size_t cap; };

2. 实现函数：
   - int intvec_init(struct IntVec *v, size_t cap);
     成功返回 0，失败返回 -1。
   - int intvec_push(struct IntVec *v, int value);
     若 size == cap，返回 -1；否则插入并返回 0。
   - void intvec_destroy(struct IntVec *v);
     释放内存并清空字段。

3. 在 main 中测试：
   - cap = 4
   - 连续 push 5 个值
   - 打印 size 和数组内容

边界条件：
- cap == 0 时 init 也允许，但 push 必须失败。
- 函数参数为 NULL 时要直接返回错误。
*/
