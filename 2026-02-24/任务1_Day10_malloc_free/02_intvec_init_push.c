/*
Day10 练习2：动态数组初始化 + push（不扩容）

实现一个固定容量的动态数组（不使用 realloc）。

要求：
1. 定义结构体：
   struct IntVec { int *data; size_t size; size_t cap; };

2. 你要实现的函数（告诉你“函数是干嘛的”）：
   - int intvec_init(struct IntVec *v, size_t cap);
     作用：初始化一个固定容量的动态数组（申请内存、把 size 设为 0）。
     额外要求：cap==0 时不申请内存，data 必须为 NULL。
     返回值：成功 0，失败 -1。
   - int intvec_push(struct IntVec *v, int value);
     作用：往数组末尾追加一个元素（容量满了就失败）。
     返回值：成功 0，失败 -1。
   - void intvec_destroy(struct IntVec *v);
     作用：释放内存并把结构体清空，防止悬空指针。

3. 在 main 中测试：
   - cap = 4
   - 连续 push 5 个值
   - 打印 size 和数组内容

边界条件：
- cap == 0 时 init 也允许，但 push 必须失败。
- 函数参数为 NULL 时要直接返回错误。
*/
#include<stdio.h>
#include<stdlib.h>
typedef struct
{
  int *data;
  size_t size;
  size_t cap;
} IntVec;
int intvec_init(IntVec *v, size_t cap)
{
  v->data = malloc(sizeof(int) * cap);
  if(!v->data){
    return 1;
  }
  v->size = 0;
  v->cap=cap;
  return 0;
}
int intvec_push(IntVec *v, int value)
{
  
  if (v == NULL)
  {
    return;
  }
  if (v->size >= v->cap)
  {
    return 1;
  }
  int *d = v->data;
  *(d + v->size) = value;
  v->size++;
  return 0;
}
void intvec_destroy(IntVec *v)
{
  if (v == NULL)
  {
    return;
  }
  free(v->data);
  v->data = NULL;
  v->size = 0;
  v->cap = 0;
}
int main(void) {
    IntVec v;
    if (intvec_init(&v, 4) != 0) {
        printf("init failed\n");
        return 1;
    }

    for (int i = 1; i <= 5; i++) {
        int ret = intvec_push(&v, i * 10);
        printf("push %d -> %s\n", i * 10, ret == 0 ? "ok" : "fail");
    }

    printf("size=%zu cap=%zu\n", v.size, v.cap);
    for (size_t i = 0; i < v.size; i++) {
        printf("%d ", v.data[i]);
    }
    printf("\n");

    intvec_destroy(&v);
    return 0;
}
