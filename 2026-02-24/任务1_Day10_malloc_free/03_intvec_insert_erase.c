/*
Day10 练习3：动态数组插入/删除（不扩容）

在已有的 IntVec 结构体基础上，实现插入与删除。

要求：
1. 你要实现的函数（告诉你“函数是干嘛的”）：
   - int intvec_insert(struct IntVec *v, size_t index, int value);
     作用：在指定位置插入一个元素（容量满了或位置不合法就失败）。
     返回值：成功 0，失败 -1。
   - int intvec_erase(struct IntVec *v, size_t index);
     作用：删除指定位置的元素（位置不合法就失败）。
     返回值：成功 0，失败 -1。

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
- v == NULL 必须直接返回 -1。
- 插入时 size == cap 必须返回 -1（今天不扩容）。
- 允许 index == size（等价于 push）；index > size 必须失败。
*/
#include <stdio.h>
#include <stdlib.h>
typedef struct
{
   int *data;
   size_t size;
   size_t cap;
} IntVec;
int intvec_init(IntVec *v, size_t cap)
{  if(v==NULL)return -1;
   if(cap==0)return -1;
   v->data = malloc(sizeof(int) * cap);
   if (!v->data)
   {
      return -1;
   }
   v->size = 0;
   v->cap = cap;
   return 0;
}
int intvec_push(IntVec *v, int value)
{

   if (v == NULL)
   {
      return -1;
   }
   if (v->size >= v->cap)
   {
      return -1;
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
int intvec_insert(IntVec *v, size_t index, int value)
{  if(v==NULL)return -1;
   if (index > v->size)
   { 
      return -1;
   }
   int *data = v->data;
   for(size_t i=v->size;i>index;i--){
      *(data+i)=*(data+i-1);
   }
   *(data+index)=value;
   v->size++;
   return 0;
}
int intvec_erase(IntVec *v, size_t index)
{   if(v==NULL)return -1;
   if (index >= v->size||index>=v->cap)
   {
      return -1;
   }
   int *data = v->data;
   for(size_t i=index;i<v->size-1;i++){
      *(data+i)=*(data+i+1);
   }
   v->size--;
   return 0;
}
int main(void)
{
   IntVec v;
   if (intvec_init(&v, 8) != 0) {
      printf("init failed\n");
      return 1;
   }

   intvec_push(&v, 1);
   intvec_push(&v, 2);
   intvec_push(&v, 3);
   intvec_push(&v, 4);

   intvec_insert(&v, 0, 99);
   intvec_insert(&v, 2, 88);
   intvec_erase(&v, 1);

   for (size_t i = 0; i < v.size; i++) {
      printf("%d ", v.data[i]);
   }
   printf("\n");

   intvec_destroy(&v);
   return 0;
}
