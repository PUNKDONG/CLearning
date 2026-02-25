/*
题2：动态数组 push_back（扩容策略 + 溢出）

实现结构体：
typedef struct {
    int *data;
    size_t size;
    size_t cap;
} Vec;

实现函数：
int vec_push(Vec *v, int x);

要求：
1. `cap == 0` 时先分配一个初始容量（例如 4）。
2. `size == cap` 时扩容（例如 `cap * 2`）。
3. 扩容计算必须做溢出检查：`new_cap * sizeof(int)` 不能溢出。
4. `realloc` 失败时返回 `-1`，并且 `v` 保持原状；成功返回 `0` 并插入元素。

自测：
- 连续 push 1000 次，size 正确、数据不乱、cap 合理增长。
*/
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h> 
typedef struct {
    int *data;
    size_t size;
    size_t cap;
} Vec;
int vec_push(Vec *v, int x){
    if(v==NULL)return -1;
    if(v->cap==0)return -1;
    int*data=v->data;
    if(v->size+1==v->cap){
        //扩容
        if(v->cap*2>SIZE_MAX/sizeof(int)){
            printf("扩容溢出");
            return -1;
        }
        int *tmp=realloc(data,v->cap*2);
        if(tmp==NULL){
            printf("扩容失败");
            return -1;
        }
        v->data=tmp;
        v->cap=2*v->cap;
    }
    *(data+v->size)=x;
    v->size++;
    return 0;
}
int main(void){
    Vec v = {0};

    // case1: 基础写入（不触发扩容）
    v.cap = 8;
    v.data = (int *)malloc(v.cap * sizeof(int));
    if (v.data == NULL) {
        printf("init malloc failed\n");
        return 1;
    }

    printf("case1: basic push\n");
    for (int i = 1; i <= 3; i++) {
        int ret = vec_push(&v, i * 10);
        printf("push %d -> ret=%d, size=%zu, cap=%zu\n", i * 10, ret, v.size, v.cap);
    }
    printf("data: ");
    for (size_t i = 0; i < v.size; i++) {
        printf("%d ", v.data[i]);
    }
    printf("\n\n");

    free(v.data);
    v.data = NULL;
    v.size = 0;
    v.cap = 0;

    // case2: 扩容触发测试（如果这里出错，问题大概率在扩容逻辑）
    v.cap = 4;
    v.data = (int *)malloc(v.cap * sizeof(int));
    if (v.data == NULL) {
        printf("init malloc failed\n");
        return 1;
    }
    printf("case2: trigger resize\n");
    for (int i = 1; i <= 6; i++) {
        int ret = vec_push(&v, i);
        printf("push %d -> ret=%d, size=%zu, cap=%zu\n", i, ret, v.size, v.cap);
        if (ret != 0) break;
    }
    printf("data: ");
    for (size_t i = 0; i < v.size; i++) {
        printf("%d ", v.data[i]);
    }
    printf("\n\n");

    free(v.data);
    v.data = NULL;
    v.size = 0;
    v.cap = 0;

    // case3: 边界输入（cap=0）
    printf("case3: cap=0 input\n");
    int ret = vec_push(&v, 123);
    printf("ret=%d, size=%zu, cap=%zu\n", ret, v.size, v.cap);
    if (v.data != NULL) {
        free(v.data);
        v.data = NULL;
    }

    return 0;
}
