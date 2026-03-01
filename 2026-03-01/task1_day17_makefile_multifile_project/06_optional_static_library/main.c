#include <stdio.h>
#include "math_utils.h"

int main(void) {
    int a = 100;
    int b = 64;

    printf("add=%d\n", add(a, b));
    printf("diff=%d\n", abs_diff(a, b));
    printf("gcd=%d\n", gcd(a, b));

    return 0;
}
