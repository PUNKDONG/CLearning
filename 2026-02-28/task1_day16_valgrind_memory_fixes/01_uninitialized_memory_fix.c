/*
Day16 练习1（故意带 bug）：
目标：用 valgrind 抓到“使用未初始化内存”并修复。

你要做的事：
1) 先按原样编译并运行 valgrind（不要先改代码）。
2) 定位报错行。
3) 修复后再次跑 valgrind，对比前后日志。
*/

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t n = 8;
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < 8; i++) {
        arr[i] = (int)(i + 1);
    }
    /* bug: arr[4..7] 未初始化 */

    int sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += arr[i]; /* 这里会读到未初始化值 */
    }

    if (sum % 2 == 0) { /* 条件判断也依赖未初始化值 */
        puts("sum is even");
    } else {
        puts("sum is odd");
    }

    free(arr);
    return 0;
}
