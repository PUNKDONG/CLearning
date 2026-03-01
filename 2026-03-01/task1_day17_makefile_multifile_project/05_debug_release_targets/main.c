#include <stdio.h>
#include "math_utils.h"

int main(void) {
    int values[] = {3, 8, 2, 9, 4};
    int n = (int)(sizeof(values) / sizeof(values[0]));

    printf("sum = %d\n", sum_array(values, n));
    printf("max = %d\n", max_array(values, n));

    return 0;
}
