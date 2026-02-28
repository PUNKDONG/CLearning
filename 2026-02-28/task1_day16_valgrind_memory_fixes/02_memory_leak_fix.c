/*
Day16 练习2（故意带 bug）：
目标：用 valgrind 抓到内存泄漏并修复。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *build_message(const char *name) {
    char *buf = (char *)malloc(128);
    if (buf == NULL) {
        return NULL;
    }
    snprintf(buf, 128, "hello, %s", name);
    return buf;
}

int main(void) {
    int *scores = (int *)malloc(10 * sizeof(int));
    char *msg = build_message("valgrind");
    if (scores == NULL || msg == NULL) {
        free(scores);
        free(msg);
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        scores[i] = i * 10;
    }

    puts(msg);
    printf("score[3] = %d\n", scores[3]);

    free(scores);
    free(msg);
    /* bug: msg 没有 free，造成 definitely lost */
    return 0;
}
