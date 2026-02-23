# Day11：realloc 与内存扩容（含边界处理）

## 1. 今日目标
- 会正确使用 `realloc` 扩容/缩容。
- 理解 `realloc` 失败的**安全写法**。
- 为动态数组补全扩容与边界处理。

## 2. realloc 必知规则（工作中必用）
### 2.1 行为规则
- `realloc(p, new_size)` 可能返回**新地址**，旧指针可能失效。
- 失败时返回 `NULL`，**旧内存仍然有效**。
- `realloc(NULL, n)` 等价于 `malloc(n)`。
- `realloc(p, 0)` 行为等价 `free(p)`（实现相关），建议自己显式 `free`。

### 2.2 安全写法（必须用）
```c
int *tmp = realloc(p, new_size);
if (!tmp) {
    // 失败：旧指针 p 仍然可用，不能丢
    return -1;
}
p = tmp; // 只在成功后更新
```

### 2.3 扩容策略
- 常用规则：`cap = (cap == 0) ? 1 : cap * 2`。
- 扩容前检查溢出：`cap > SIZE_MAX / 2` 则拒绝扩容。

## 3. 动态数组扩容后的边界
- `insert`：index 必须 `<= size`。
- `erase`：index 必须 `< size`。
- `push`：`size == cap` 时先扩容。
- `cap == 0` 时 push 触发扩容为 1。

## 4. 常见错误（必须避免）
- 直接 `p = realloc(p, ...)`，失败时丢失旧指针。
- 扩容后忘记更新 `cap`。
- 扩容后仍用旧指针。
- 没有检查 `index` 边界。

## 5. 今日练习说明
- 练习集中在 `realloc` 与扩容边界。
- 题目在 `.c` 文件注释中。
- 今日共 3 题，覆盖全部知识点（由易到难）。
