/*
Day13 练习3（挑战）：删除链表中所有值为 x 的节点

题目：
实现函数：
    struct Node *remove_all_x(struct Node *head, int x);

函数功能：
- 删除链表中所有 data == x 的节点。
- 返回删除后的新头指针。

要求：
1. 必须正确处理“头节点连续命中”场景（例如 x x x 5 6）。
2. 删除节点后必须 free 对应内存，防止内存泄漏。
3. 不允许访问已 free 的节点（避免悬空指针）。
4. 其他值节点的相对顺序保持不变。

边界条件（必须覆盖）：
1. 空链表。
2. 所有节点都等于 x（结果应为 NULL）。
3. 没有节点等于 x（链表应保持不变）。
4. 头部、中间、尾部都出现 x。

验收标准：
1. 输入：1->2->3->2->4, x=2，输出：1->3->4。
2. 输入：2->2->2, x=2，输出：NULL。
*/
#include <stdio.h>
#include <stdlib.h>
typedef struct Node
{
    int data;
    struct Node *next;
} Node;
Node *remove_all_x(Node *head, int x)
{   Node dummy={0,head};
    Node *pre = &dummy;
    Node *cur=head;
    while(cur!=NULL){
        Node *next=cur->next;
        if(cur->data==x){
            pre->next=next;
            free(cur);
        }else{
            pre=cur;
        }
        cur=next;
    }
    return dummy.next;
}
int main(void)
{
    Node *head = NULL;
    Node *p = NULL;

    // case1: 空链表
    head = NULL;
    head = remove_all_x(head, 2);
    printf("case1 空链表, x=2: ");
    if (head == NULL) {
        printf("NULL\n");
    } else {
        printf("非 NULL（错误）\n");
    }

    // case2(验收标准2): 2->2->2, x=2, 期望 NULL
    Node *a1 = (Node *)malloc(sizeof(Node));
    Node *a2 = (Node *)malloc(sizeof(Node));
    Node *a3 = (Node *)malloc(sizeof(Node));
    a1->data = 2; a1->next = a2;
    a2->data = 2; a2->next = a3;
    a3->data = 2; a3->next = NULL;
    head = a1;
    head = remove_all_x(head, 2);
    printf("case2 2->2->2, x=2: ");
    if (head == NULL) {
        printf("NULL\n");
    } else {
        printf("非 NULL（错误）\n");
    }

    // case3: 没有节点等于 x，链表应保持不变
    Node *b1 = (Node *)malloc(sizeof(Node));
    Node *b2 = (Node *)malloc(sizeof(Node));
    Node *b3 = (Node *)malloc(sizeof(Node));
    b1->data = 1; b1->next = b2;
    b2->data = 3; b2->next = b3;
    b3->data = 4; b3->next = NULL;
    head = b1;
    head = remove_all_x(head, 2);
    printf("case3 1->3->4, x=2: ");
    p = head;
    while (p != NULL) {
        printf("%d", p->data);
        if (p->next != NULL) {
            printf("->");
        }
        p = p->next;
    }
    printf("\n");
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }

    // case4: 头部/中间/尾部都出现 x
    Node *c1 = (Node *)malloc(sizeof(Node));
    Node *c2 = (Node *)malloc(sizeof(Node));
    Node *c3 = (Node *)malloc(sizeof(Node));
    Node *c4 = (Node *)malloc(sizeof(Node));
    Node *c5 = (Node *)malloc(sizeof(Node));
    c1->data = 2; c1->next = c2;
    c2->data = 1; c2->next = c3;
    c3->data = 2; c3->next = c4;
    c4->data = 3; c4->next = c5;
    c5->data = 2; c5->next = NULL;
    head = c1;
    head = remove_all_x(head, 2);
    printf("case4 2->1->2->3->2, x=2: ");
    p = head;
    while (p != NULL) {
        printf("%d", p->data);
        if (p->next != NULL) {
            printf("->");
        }
        p = p->next;
    }
    printf("  (期望 1->3)\n");
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }

    // case5(验收标准1): 1->2->3->2->4, x=2, 期望 1->3->4
    Node *d1 = (Node *)malloc(sizeof(Node));
    Node *d2 = (Node *)malloc(sizeof(Node));
    Node *d3 = (Node *)malloc(sizeof(Node));
    Node *d4 = (Node *)malloc(sizeof(Node));
    Node *d5 = (Node *)malloc(sizeof(Node));
    d1->data = 1; d1->next = d2;
    d2->data = 2; d2->next = d3;
    d3->data = 3; d3->next = d4;
    d4->data = 2; d4->next = d5;
    d5->data = 4; d5->next = NULL;
    head = d1;
    head = remove_all_x(head, 2);
    printf("case5 1->2->3->2->4, x=2: ");
    p = head;
    while (p != NULL) {
        printf("%d", p->data);
        if (p->next != NULL) {
            printf("->");
        }
        p = p->next;
    }
    printf("  (期望 1->3->4)\n");
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }

    return 0;
}
