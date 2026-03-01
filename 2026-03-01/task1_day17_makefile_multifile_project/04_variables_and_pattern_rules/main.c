#include <stdio.h>
#include "math_utils.h"
#include "stats_utils.h"

int main(void) {
    int x = 9;
    int y = 21;

    printf("add=%d\n", add(x, y));
    printf("max=%d\n", max_of_two(x, y));
    printf("diff=%d\n", abs_diff(x, y));
    printf("average=%d\n", average_int(x, y));

    return 0;
}
