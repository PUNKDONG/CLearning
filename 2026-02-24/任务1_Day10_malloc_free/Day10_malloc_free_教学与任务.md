# Day10：malloc/free 基础 + 动态数组增删（不扩容）

## 0. 本文使用方式（先看这段）
- 这是“可执行”的学习文档：看完每一节就能直接写代码。
- 不追求记概念名词，追求“写得对、跑得通”。
- 今天**禁止使用 `realloc`**，只做 `malloc/free` + 固定容量的动态数组。

---

## 1. 今日目标（要达成的能力）
1. 能正确分配/释放堆内存，不发生泄漏、重复释放、越界访问。
2. 能分清 `malloc` 与 `calloc`，知道什么时候用哪一个。
3. 能用 “指针 + size + cap” 实现动态数组的插入/删除（不扩容）。

完成标准：
- 你能写出 `malloc/free` 的正确用法，并解释每一行为什么这样写。
- 你能写出 `IntVec` 的 init/push/insert/erase/destroy，并覆盖边界条件。

---

## 2. 先搞清楚：内存到底在哪
你写的变量会放在不同区域，今天只需要记 2 个：

### 2.1 栈（stack）
- 位置：函数内部的局部变量（例如 `int x;`）。
- 生命周期：函数一结束，自动回收。
- 你不需要手动释放。

### 2.2 堆（heap）
- 位置：通过 `malloc/calloc` 手动申请。
- 生命周期：只有你 `free` 才会释放。
- **忘记 `free` = 内存泄漏**。

一句话理解：
> 栈是“自动临时工”，堆是“你自己招的人”，你不解雇就一直占着。

---

## 3. malloc/calloc/free 的最低限度知识
### 3.1 函数签名（必须会看）
```c
#include <stdlib.h>

void *malloc(size_t size);
void *calloc(size_t n, size_t size);
void free(void *p);
```

### 3.2 参数与返回值（必须理解）
- `size_t`：表示“大小/长度”的无符号整数类型。
- `malloc(size)`：申请 `size` 字节，不初始化。
- `calloc(n, size)`：申请 `n * size` 字节，并把**全部字节清零**。
- 失败都返回 `NULL`。

**重点：`calloc` 的两个参数必须是整数**，通常写成：
```c
int *a = calloc(n, sizeof(int));
```

### 3.3 malloc vs calloc（一句话区别）
- `malloc`：只给内存，里面是“脏数据”。
- `calloc`：给内存并清零（通常更安全）。

例子：
```c
int *a = malloc(n * sizeof(int)); // 不清零
int *b = calloc(n, sizeof(int));  // 清零
```

### 3.4 free 的规则（必须牢记）
1. 只能释放“从 malloc/calloc/realloc 得到的指针”。
2. **同一指针只能 free 一次**。
3. `free(NULL)` 安全。
4. `free` 后指针变“悬空”，再访问就是未定义行为。

---

## 4. malloc/calloc 的正确写法（避免新手错误）
### 4.1 推荐写法
```c
int *p = malloc(n * sizeof(*p));
if (!p) {
    // 申请失败，必须处理
}
```

为什么用 `sizeof(*p)`？
- 这样类型一致，哪怕你以后把 `p` 改成别的类型也不容易写错。

### 4.2 典型错误
**错误 1：不检查返回值**
```c
int *p = malloc(n * sizeof(int));
*p = 123; // 如果 p==NULL 直接崩溃
```

**错误 2：写错 sizeof**
```c
int *p = malloc(n * sizeof(int*)); // 错：分配的是指针大小
```

**错误 3：calloc 参数乱写**
```c
calloc(100, "xxx"); // 错：第二个参数必须是整数
```

---

## 5. 动态数组模型（不扩容版）
### 5.1 数据结构
```c
struct IntVec {
    int *data;    // 堆上数组首地址
    size_t size;  // 已使用元素个数
    size_t cap;   // 总容量
};
```

### 5.2 永远要保持的约束
- `size <= cap` 必须成立。
- `cap == 0` 时 `data` 可以是 NULL。

### 5.3 操作规则（今天只做这些）
1. **init**
   - 分配 `cap * sizeof(int)` 字节。
   - `size = 0`。
2. **push**（在末尾添加）
   - 如果 `size == cap`，直接失败。
   - 否则 `data[size] = value; size++`。
3. **insert**（指定位置插入）
   - 合法 index：`0 <= index <= size`。
   - 先把尾部元素向后移动一格，再写入。
4. **erase**（删除指定位置）
   - 合法 index：`0 <= index < size`。
   - 把 `index+1` 之后的元素往前移动一格。
5. **destroy**
   - `free(data)`，`data = NULL`，`size = cap = 0`。

---

## 6. 插入/删除的“机械步骤”（别怕照着写）
### 6.1 insert 的移动逻辑
假设数组是：`[1,2,3,4]`，`size=4`，在 index=1 插入 `99`。

步骤：
1. 从最后一个元素开始，向后挪一格。
2. 最终变成：`[1,_,2,3,4]`。
3. 在空位写 `99`。

伪代码：
```c
for (i = size; i > index; i--) {
    data[i] = data[i-1];
}
data[index] = value;
size++;
```

### 6.2 erase 的移动逻辑
假设数组：`[1,2,3,4]`，删除 index=1。

步骤：
1. 从 index+1 开始往前覆盖。
2. 最终变成：`[1,3,4,_]`，`size--`。

伪代码：
```c
for (i = index; i < size-1; i++) {
    data[i] = data[i+1];
}
size--;
```

---

## 7. 边界条件（面试/实战都常考）
1. `cap == 0`：
   - init 后 `data` 可以是 `NULL`；
   - push/insert 直接失败。

2. `index` 越界：
   - insert 要求 `index <= size`，否则失败。
   - erase 要求 `index < size`，否则失败。

3. `malloc` 失败：
   - 立刻返回错误，不要继续使用指针。

4. `free` 后访问：
   - 这是未定义行为，**一定要避免**。

---

## 8. 最小可运行模板（帮你快速起步）
```c
#include <stdio.h>
#include <stdlib.h>

struct IntVec {
    int *data;
    size_t size;
    size_t cap;
};

int intvec_init(struct IntVec *v, size_t cap) {
    if (!v) return -1;
    v->data = (cap == 0) ? NULL : malloc(cap * sizeof(int));
    if (cap != 0 && !v->data) return -1;
    v->size = 0;
    v->cap = cap;
    return 0;
}

void intvec_destroy(struct IntVec *v) {
    if (!v) return;
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}
```

说明：
- 这段模板不是让你复制粘贴完成题目，而是让你知道“正确结构长什么样”。

---

## 9. 今日练习说明（3 题，覆盖全部知识点）
- **禁止使用 `realloc`**。
- 题目在 `.c` 文件注释里，按要求自己写完整程序。
- 难度递进：
  1) malloc/free 基础
  2) IntVec 初始化 + push
  3) IntVec 插入 + 删除

---

## 10. 自查清单（写完就对照一遍）
- [ ] `malloc/calloc` 返回值有检查。
- [ ] `free` 只释放堆内存，且只释放一次。
- [ ] `size` 和 `cap` 关系始终成立。
- [ ] insert/erase 的 index 边界都判断了。
- [ ] 失败分支能安全退出（不会崩）。

如果你在某一步卡住，直接告诉我你卡在哪个点，我给你更具体的拆解。
