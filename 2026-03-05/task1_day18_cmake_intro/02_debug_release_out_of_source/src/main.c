#include <stdio.h>

#include "math_utils.h"

int main(void) {
    int a = 4;
    int b = 9;

    printf("square(%d) = %d\n", a, square(a));
    printf("max2(%d, %d) = %d\n", a, b, max2(a, b));
    return 0;
}
