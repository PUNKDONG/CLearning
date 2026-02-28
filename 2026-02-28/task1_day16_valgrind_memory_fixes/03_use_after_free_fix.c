/*
Day16 练习3（故意带 bug）：
目标：用 valgrind 抓到 use-after-free 并修复。
*/

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = (int *)malloc(3 * sizeof(int));
    if (p == NULL) {
        perror("malloc");
        return 1;
    }

    p[0] = 10;
    p[1] = 20;
    p[2] = 30;

 
    printf("after free p[0] = %d\n", p[0]);
    p[1] = 99;
   free(p);

    return 0;
}
