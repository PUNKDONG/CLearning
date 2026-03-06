#include <stdio.h>

#include "utils.h"

int main(void) {
    int a = 2;
    int b = 5;
    int c = 8;

    print_banner();
    printf("sum3(%d, %d, %d) = %d\n", a, b, c, sum3(a, b, c));
    return 0;
}
