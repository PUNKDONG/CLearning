/*
Day13 练习2（进阶）：原地反转单链表

题目：
实现函数：
    struct Node *reverse_list(struct Node *head);

函数功能：
- 将链表原地反转，并返回“新的头指针”。
- 不能新建第二条链表。

要求：
1. 必须通过指针重连完成（prev / cur / next 思路）。
2. 空链表或单节点链表，返回原 head。
3. 反转后链表中节点总数不变，且每个节点地址应与反转前相同。

边界条件：
1. head == NULL。
2. 只有 1 个节点。
3. 多节点链表（例如 1->2->3->4）。

验收标准：
1. 1->2->3->4 反转后应为 4->3->2->1。
2. 没有内存泄漏、没有野指针访问。
3. 编译无 warning（建议 -Wall -Wextra -std=c11）。
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *reverse_list(Node *head)
{
    if(head==NULL||head->next==NULL){
        return head;
    }
    Node *pre=NULL;
    Node*p=head;
    while(p->next!=NULL){
      Node *next=p->next;
      p->next=pre;
      pre=p;
      p=next; 
    }
    p->next=pre;
    return p;
}
int main(void)
{
    Node a = {11, NULL};
    Node b = {22, NULL};
    Node c = {33, NULL};
    Node d = {44, NULL};
    Node e = {55, NULL};
    a.next = &b;
    b.next = &c;
    c.next = &d;
    d.next = &e;
    Node dummy = {0, &a};
    Node *p = &dummy;
    while (p->next != NULL)
    {
        printf("%d->", p->data);
        p = p->next;
    }
    printf("%d\n", p->data);
    dummy.next = reverse_list(dummy.next);
    p = &dummy;
    while (p->next != NULL)
    {
        printf("%d->", p->data);
        p = p->next;
    }
    printf("%d\n", p->data);

    // 以下为边界条件测试（追加，不改你原有逻辑）
    printf("\n=== boundary tests ===\n");

    // 1) head == NULL（用子进程隔离崩溃，避免主进程被中断）
    pid_t pid = fork();
    if (pid == 0) {
        Node *h0 = NULL;
        Node *r0 = reverse_list(h0);
        (void)r0;
        _exit(0);
    } else if (pid > 0) {
        int st = 0;
        waitpid(pid, &st, 0);
        if (WIFSIGNALED(st)) {
            printf("[FAIL] case1 head==NULL: crashed by signal %d\n", WTERMSIG(st));
        } else {
            printf("[PASS] case1 head==NULL\n");
        }
    } else {
        printf("[FAIL] case1 head==NULL: fork failed\n");
    }

    // 2) 只有 1 个节点
    Node one = {42, NULL};
    Node *r1 = reverse_list(&one);
    if (r1 == &one && r1->data == 42 && r1->next == NULL) {
        printf("[PASS] case2 one node\n");
    } else {
        printf("[FAIL] case2 one node\n");
    }

    // 3) 多节点链表（1->2->3->4）
    Node x1 = {1, NULL};
    Node x2 = {2, NULL};
    Node x3 = {3, NULL};
    Node x4 = {4, NULL};
    x1.next = &x2;
    x2.next = &x3;
    x3.next = &x4;
    Node *r2 = reverse_list(&x1);
    if (r2 == &x4 && x4.next == &x3 && x3.next == &x2 && x2.next == &x1 && x1.next == NULL) {
        printf("[PASS] case3 1->2->3->4\n");
    } else {
        printf("[FAIL] case3 1->2->3->4\n");
    }

    return 0;
}
