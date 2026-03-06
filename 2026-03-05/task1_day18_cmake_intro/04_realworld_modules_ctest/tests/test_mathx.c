#include <stdio.h>

#include "mathx.h"

int main(void) {
    const int a[] = {1, 2, 3};
    const int b[] = {5, -2, 9};

    if (sum_array(a, 3) != 6) {
        return 1;
    }
    if (sum_array(b, 3) != 12) {
        return 2;
    }

    puts("test_mathx passed");
    return 0;
}
