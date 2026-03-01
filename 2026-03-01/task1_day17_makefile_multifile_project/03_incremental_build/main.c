#include <stdio.h>
#include "math_utils.h"

int main(void) {
    int a = 40;
    int b = 13;

    printf("sum=%d\n", add(a, b));
    printf("max=%d\n", max_of_two(a, b));
    printf("diff=%d\n", abs_diff(a, b));
    return 0;
}
