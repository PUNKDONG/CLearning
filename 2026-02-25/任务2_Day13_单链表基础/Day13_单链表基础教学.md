# Day13 单链表基础教学

## 1. 今天要掌握什么

今天目标是把单链表最核心的操作打通：

1. 结构体定义（节点定义）。
2. 初始化（空链表）。
3. 动态内存分配（`malloc`）创建节点。
4. 创建链表（头插法、尾插法）。
5. 遍历、查找。
6. 插入节点、删除节点。
7. 能识别并规避常见指针坑：断链、野指针、内存泄漏。

---

## 2. 单链表是什么

单链表由一串节点组成，每个节点至少包含：

1. 数据域（存值）。
2. 指针域（指向下一个节点）。

典型定义（C11）：

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;              // 数据域
    struct Node *next;     // 指针域：指向下一个节点
} Node;
```

---

## 3. “头结点”与“首元节点”的区别（重点）

这是很多初学者最容易混淆的点。

1. 头结点（dummy head）
- 是“额外加出来”的一个节点。
- 通常不存业务数据（或数据无意义）。
- 作用是统一边界处理，特别是插入/删除第一个数据节点时更简单。

2. 首元节点（first real node）
- 第一个真正存业务数据的节点。
- 没有头结点的链表里，`head` 直接指向首元节点。
- 有头结点的链表里，`head->next` 才是首元节点。

示意：

- 无头结点：`head -> [10] -> [20] -> NULL`
- 有头结点：`head(dummy) -> [10] -> [20] -> NULL`

工程建议：
- 初学阶段可以都练。
- 做“删除指定值、在头部插入”这类题时，有头结点通常更稳。

---

## 4. 初始化链表

## 4.1 无头结点初始化

```c
Node *head = NULL;  // 空链表，head 直接表示首元节点
```

## 4.2 有头结点初始化

```c
Node *head = (Node *)malloc(sizeof(Node));
if (head == NULL) {
    // 申请失败要处理
    return -1;
}
head->next = NULL;  // 头结点后面暂时没有数据节点
```

注意：
- `malloc` 后必须检查返回值。
- 头结点本身也属于堆内存，最后也要释放。

---

## 5. 动态创建节点（malloc）

推荐统一写一个“节点工厂函数”，避免重复代码。

```c
static Node *create_node(int value) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        return NULL;
    }
    n->data = value;
    n->next = NULL;
    return n;
}
```

为什么必须这样写：
1. 申请失败时能及时返回。
2. `next` 先置 `NULL`，减少野指针风险。

---

## 6. 创建链表：头插法与尾插法

## 6.1 头插法（新节点插到最前面）

特点：
- 时间复杂度 O(1) 每次插入都快。
- 最终顺序与输入顺序相反。

```c
// 无头结点版本
int push_front(Node **head, int value) {
    Node *n = create_node(value);
    if (n == NULL) {
        return -1;
    }
    n->next = *head;
    *head = n;
    return 0;
}
```

关键点：
- 传 `Node **head`，因为要修改调用者手里的头指针。

## 6.2 尾插法（新节点插到最后）

特点：
- 保持输入顺序。
- 若每次都从头找尾，单次 O(n)。

```c
// 无头结点版本
int push_back(Node **head, int value) {
    Node *n = create_node(value);
    if (n == NULL) {
        return -1;
    }

    if (*head == NULL) {
        *head = n;
        return 0;
    }

    Node *cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = n;
    return 0;
}
```

---

## 7. 遍历与查找

## 7.1 遍历打印

```c
void print_list(const Node *head) {
    const Node *cur = head;
    while (cur != NULL) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}
```

## 7.2 按值查找（返回第一个命中的节点地址）

```c
Node *find_first(Node *head, int x) {
    Node *cur = head;
    while (cur != NULL) {
        if (cur->data == x) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}
```

---

## 8. 插入与删除（指针操作核心）

## 8.1 在指定位置后插入

场景：已知前驱节点 `prev`，在其后插入新节点。

```c
int insert_after(Node *prev, int value) {
    if (prev == NULL) {
        return -1;
    }
    Node *n = create_node(value);
    if (n == NULL) {
        return -1;
    }

    n->next = prev->next;
    prev->next = n;
    return 0;
}
```

顺序不能写反：
- 必须先 `n->next = prev->next`，再 `prev->next = n`。
- 写反会断链。

## 8.2 删除第一个值为 x 的节点（无头结点）

```c
int remove_first(Node **head, int x) {
    if (head == NULL || *head == NULL) {
        return -1;
    }

    Node *cur = *head;
    Node *prev = NULL;

    while (cur != NULL) {
        if (cur->data == x) {
            if (prev == NULL) {
                // 删除首元节点
                *head = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }

    return -1;
}
```

---

## 9. 常见坑与规避方式（务必记住）

## 9.1 断链

错误本质：
- 改指针顺序不当，导致后续节点地址丢失。

规避：
- 涉及重连时，先保存旧链路，再改当前链接。

## 9.2 野指针

典型来源：
1. 指针未初始化就使用。
2. `free` 后继续访问。

规避：
- 新指针先置 `NULL`。
- `free(p); p = NULL;`。

## 9.3 内存泄漏

典型来源：
- 删除节点时只改链接，不 `free`。
- 程序结束前忘记释放整条链表。

规避：
- 所有“创建节点”的路径，最终都要有匹配释放。
- 写一个统一销毁函数。

```c
void destroy_list(Node **head) {
    if (head == NULL) {
        return;
    }
    Node *cur = *head;
    while (cur != NULL) {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
}
```

---

## 10. 调试与自测建议

编译建议（C11 + 警告）：

```bash
gcc -std=c11 -Wall -Wextra -g your_file.c -o your_app
```

建议每次做完操作都打印链表：
1. 插入后打印。
2. 删除后打印。
3. 反转后打印。

单链表 bug 的定位核心：
- 不要只看最终结果。
- 看每一步链接关系是否符合预期。

---

## 11. 今天的实践目标

如果你能独立写出这三类函数，就说明 Day13 过关：

1. `size_t list_length(const Node *head);`
2. `Node *reverse_list(Node *head);`
3. `Node *remove_all_x(Node *head, int x);`

这三个函数分别训练：
- 基础遍历能力。
- 指针重连能力。
- 复杂边界处理能力（尤其删除头节点）。
