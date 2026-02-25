/*
题1：安全扩容函数（基础必会）

实现函数：
int grow_int_array(int **p, size_t old_n, size_t new_n);

要求：
1. 把 `*p` 指向的 `int` 数组从 `old_n` 扩容/缩容到 `new_n`（元素个数）。
2. 必须用临时指针接 `realloc`，失败时不得丢失原指针。
3. 若 `new_n > old_n`，新增区间全部初始化为 `0`。
4. 返回值：成功返回 `0`，失败返回 `-1`（失败时数组内容保持不变）。

测试提示：
- `old_n=3,new_n=10` 后，原 3 个元素保留，后面 7 个是 0。
- 模拟失败（比如申请很大）时，原指针仍可用。
*/
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h> 
int grow_int_array(int **p, size_t old_n, size_t new_n){
    if(p==NULL)return -1;
    if(new_n>SIZE_MAX/sizeof(int))return -1;
    int*temp=realloc(*p,new_n*sizeof(int));
    if(temp==NULL){
        return -1;
    }
    if(new_n>old_n){
        for(size_t i=old_n;i<new_n;i++){
            *(temp+i)=0;
        }
    }
    
    *p=temp;
    return 0;
}
int main(void){
    size_t n = 3;
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("init malloc failed\n");
        return 1;
    }

    arr[0] = 11;
    arr[1] = 22;
    arr[2] = 33;

    printf("case1: grow 3 -> 10\n");
    if (grow_int_array(&arr, 3, 10) != 0) {
        printf("grow failed\n");
        free(arr);
        return 1;
    }

    int keep_ok = (arr[0] == 11 && arr[1] == 22 && arr[2] == 33);
    int zero_ok = 1;
    for (size_t i = 3; i < 10; i++) {
        if (arr[i] != 0) {
            zero_ok = 0;
            break;
        }
    }
    printf("preserve_old=%s, new_zero=%s\n", keep_ok ? "OK" : "FAIL", zero_ok ? "OK" : "FAIL");
    printf("array after grow: ");
    for (size_t i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    arr[3] = 44;
    arr[4] = 55;

    printf("case2: shrink 10 -> 4\n");
    if (grow_int_array(&arr, 10, 4) != 0) {
        printf("shrink failed\n");
        free(arr);
        return 1;
    }
    printf("array after shrink: ");
    for (size_t i = 0; i < 4; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("case3: same size 4 -> 4\n");
    if (grow_int_array(&arr, 4, 4) != 0) {
        printf("same-size realloc failed\n");
        free(arr);
        return 1;
    }
    printf("same-size pass\n");

    free(arr);
    arr = NULL;
    return 0;
}
