/*
题3：缩容与“内部指针失效”（进阶坑点）

你有一个数组 `int *a`，并且你额外保存了一个指向内部的指针：
int *mid = &a[k];  // 0 < k < n

现在你需要实现：
int shrink_keep_index(int **a, size_t *n, size_t new_n, size_t k, int **mid);

要求：
1. 把数组缩容到 `new_n`（`new_n <= *n`）。
2. 缩容后让 `*mid` 仍然指向“缩容后数组中第 k 个元素”（如果 k 仍合法）。
3. 必须正确处理 `realloc` 发生搬家导致旧地址无效的问题：不能直接用旧 `mid`。
4. 若 `k >= new_n`，则缩容后 `*mid` 置为 `NULL`。
5. 失败返回 `-1`，成功返回 `0`。

提示：
- 关键在于：保存索引而不是保存旧地址，成功后用新基址重算 `mid`。
*/
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h> 
int shrink_keep_index(int **a, size_t *n, size_t new_n, size_t k, int **mid){
    if(a==NULL||n==NULL||mid==NULL)return -1;

    if(new_n>*n){
return -1;
    }
    int*tmp=realloc(*a,new_n);
    if(tmp==NULL){
        return -1;
    }
    *n=new_n;
    if(*a==tmp){
        if(k>=new_n){
            *mid=NULL;
        }
    }else{
    *a=tmp;
    if(k>=new_n){
            *mid=NULL;
        }else{
            
            *mid=tmp+k;
        }
    }
    return 0;
}

static int array_equal(const int *a, const int *b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

static void print_case(const char *name, int pass) {
    printf("%s: %s\n", name, pass ? "PASS" : "FAIL");
}

int main(void){
    int all_pass = 1;

    // case1: 正常缩容，k 保留在范围内
    size_t n1 = 6;
    int *a1 = (int *)malloc(n1 * sizeof(int));
    for (size_t i = 0; i < n1; i++) a1[i] = (int)(10 * (i + 1)); // 10 20 30 40 50 60
    int *mid1 = &a1[2];
    int ret1 = shrink_keep_index(&a1, &n1, 4, 2, &mid1);
    int exp1[] = {10, 20, 30, 40};
    int pass1 = (ret1 == 0 && n1 == 4 && mid1 == &a1[2] && *mid1 == 30 && array_equal(a1, exp1, 4));
    print_case("case1(valid k)", pass1);
    all_pass = all_pass && pass1;
    free(a1);

    // case2: k 越界，mid 应设为 NULL
    size_t n2 = 6;
    int *a2 = (int *)malloc(n2 * sizeof(int));
    for (size_t i = 0; i < n2; i++) a2[i] = (int)(i + 1); // 1..6
    int *mid2 = &a2[5];
    int ret2 = shrink_keep_index(&a2, &n2, 4, 5, &mid2);
    int exp2[] = {1, 2, 3, 4};
    int pass2 = (ret2 == 0 && n2 == 4 && mid2 == NULL && array_equal(a2, exp2, 4));
    print_case("case2(k>=new_n)", pass2);
    all_pass = all_pass && pass2;
    free(a2);

    // case3: new_n == n，不应破坏数据
    size_t n3 = 5;
    int *a3 = (int *)malloc(n3 * sizeof(int));
    for (size_t i = 0; i < n3; i++) a3[i] = (int)(100 + i); // 100..104
    int *mid3 = &a3[4];
    int ret3 = shrink_keep_index(&a3, &n3, 5, 4, &mid3);
    int exp3[] = {100, 101, 102, 103, 104};
    int pass3 = (ret3 == 0 && n3 == 5 && mid3 == &a3[4] && *mid3 == 104 && array_equal(a3, exp3, 5));
    print_case("case3(new_n==n)", pass3);
    all_pass = all_pass && pass3;
    free(a3);

    // case4: 非法参数（new_n > n）应失败，且状态不变
    size_t n4 = 4;
    int *a4 = (int *)malloc(n4 * sizeof(int));
    for (size_t i = 0; i < n4; i++) a4[i] = (int)(7 + i); // 7 8 9 10
    int *mid4 = &a4[1];
    int *old_a4 = a4;
    size_t old_n4 = n4;
    int *old_mid4 = mid4;
    int ret4 = shrink_keep_index(&a4, &n4, 6, 1, &mid4);
    int pass4 = (ret4 == -1 && a4 == old_a4 && n4 == old_n4 && mid4 == old_mid4 && *mid4 == 8);
    print_case("case4(new_n>n)", pass4);
    all_pass = all_pass && pass4;
    free(a4);

    // case5: NULL 参数
    size_t n5 = 3;
    int *a5 = (int *)malloc(n5 * sizeof(int));
    int *mid5 = a5;
    int p5a = (shrink_keep_index(NULL, &n5, 2, 0, &mid5) == -1);
    int p5b = (shrink_keep_index(&a5, NULL, 2, 0, &mid5) == -1);
    int p5c = (shrink_keep_index(&a5, &n5, 2, 0, NULL) == -1);
    int pass5 = p5a && p5b && p5c;
    print_case("case5(NULL args)", pass5);
    all_pass = all_pass && pass5;
    free(a5);

    printf("ALL: %s\n", all_pass ? "PASS" : "FAIL");
    return 0;
}
