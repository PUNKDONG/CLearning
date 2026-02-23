# Day10：malloc/free 基础 + 动态数组增删（不扩容）

## 1. 今日目标
- 会正确使用 `malloc/free` 管理堆内存。
- 理解“动态数组 = 指针 + size + cap”的基本模型。
- 在**不使用 `realloc`** 的前提下完成插入、删除。

## 2. 必须掌握的核心概念（工作中必用）
### 2.1 堆 vs 栈
- 栈：函数内局部变量，自动分配/回收。
- 堆：`malloc/calloc/realloc` 手动分配，**必须 `free`**。

### 2.2 `malloc`/`calloc`
- `malloc(n * sizeof(T))`：分配 `n` 个 `T` 的内存，**内容未初始化**。
- `calloc(n, sizeof(T))`：分配并**清零**。
- 失败时返回 `NULL`，必须检查。
- 推荐写法：`p = malloc(n * sizeof(*p));` 避免类型不一致。

### 2.3 `free`
- 只能 `free` 从 `malloc/calloc/realloc` 得到的指针。
- **同一块内存只能 `free` 一次**。
- `free` 后**不要再用**这个指针（use-after-free）。
- `free(p)` 后建议 `p = NULL;`。
- `free(NULL)` 是安全的。

### 2.4 常见错误（必须避免）
- 忘记 `free`（内存泄漏）。
- `free` 后继续访问。
- 多次 `free` 同一指针。
- `free` 了栈上的地址。

## 3. 动态数组（不扩容版）
### 3.1 数据结构
```
struct IntVec {
    int *data;    // 堆上数组首地址
    size_t size;  // 已使用元素个数
    size_t cap;   // 总容量
};
```

### 3.2 约束与边界
- `size <= cap` 永远成立。
- `insert`：只在 `size < cap` 时允许。
- `erase`：下标必须 `index < size`。
- `cap == 0` 时 `data` 可为 `NULL`，操作应直接失败并返回错误。

### 3.3 基本步骤
1. 初始化：`data = malloc(cap * sizeof(int))`，`size = 0`。
2. 插入：检查边界；从尾到头移动元素；写入；`size++`。
3. 删除：检查边界；从删除点后移元素；`size--`。
4. 销毁：`free(data)`，指针置 `NULL`，`size=cap=0`。

## 4. 今日练习说明
- **禁止使用 `realloc`**。
- 练习只写 C 代码，不要补题目之外的功能。
- 题目在 `.c` 文件注释中。
- 今日共 3 题，覆盖全部知识点（由易到难）。
