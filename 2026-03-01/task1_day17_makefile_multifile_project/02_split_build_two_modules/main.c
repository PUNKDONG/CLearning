#include <stdio.h>
#include "math_utils.h"

int main(void) {
    int a = 17;
    int b = 5;

    printf("a=%d, b=%d\n", a, b);
    printf("add(a, b) = %d\n", add(a, b));
    printf("max_of_two(a, b) = %d\n", max_of_two(a, b));
    printf("abs_diff(a, b) = %d\n", abs_diff(a, b));

    return 0;
}
