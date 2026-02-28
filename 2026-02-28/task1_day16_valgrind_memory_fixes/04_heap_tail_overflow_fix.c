/*
Day16 练习4（故意带 bug）：
目标：用 valgrind 抓到尾部越界读写并修复。
*/

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t n = 5;
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        arr[i] = (int)(i * 10);
    }

    //printf("tail value = %d\n", arr[n]);
    printf("tail value = %d\n", arr[n-1]);

    free(arr);
    return 0;
}
