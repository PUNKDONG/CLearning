/*
Day13 练习1（基础）：计算链表长度

题目：
实现函数：
    size_t list_length(const struct Node *head);

函数功能：
- 返回链表中“数据节点”的个数。

要求：
1. 只能遍历一次链表（while/for 都可以）。
2. 不能修改链表结构（只读遍历）。
3. 空链表返回 0。

边界条件：
1. head == NULL（空链表）。
2. 只有 1 个节点。
3. 普通多节点链表。

验收标准：
1. 输入空链表 -> 返回 0。
2. 输入 5 个节点 -> 返回 5。
3. 编译无 warning（建议 -Wall -Wextra -std=c11）。
*/
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

size_t list_length(const Node *head)
{
    size_t len = 0;
     Node *p = head;
    while (p != NULL) {
        len++;
        p = p->next;
    }
    return len;
}

int main(void)
{
    int all_pass = 1;

    // 用例1：空链表
    Node *head0 = NULL;
    size_t got0 = list_length(head0);
    if (got0 != 0) {
        printf("[FAIL] case0: got=%zu, expect=0\n", got0);
        all_pass = 0;
    } else {
        printf("[PASS] case0: empty list -> 0\n");
    }

    // 用例2：单节点链表
    Node n1 = {10, NULL};
    size_t got1 = list_length(&n1);
    if (got1 != 1) {
        printf("[FAIL] case1: got=%zu, expect=1\n", got1);
        all_pass = 0;
    } else {
        printf("[PASS] case1: one node -> 1\n");
    }

    // 用例3：5节点链表
    Node a = {1, NULL};
    Node b = {2, NULL};
    Node c = {3, NULL};
    Node d = {4, NULL};
    Node e = {5, NULL};
    a.next = &b;
    b.next = &c;
    c.next = &d;
    d.next = &e;
    size_t got2 = list_length(&a);
    if (got2 != 5) {
        printf("[FAIL] case2: got=%zu, expect=5\n", got2);
        all_pass = 0;
    } else {
        printf("[PASS] case2: five nodes -> 5\n");
    }

    if (!all_pass) {
        return 1;
    }
    printf("All tests passed.\n");
    return 0;
}
