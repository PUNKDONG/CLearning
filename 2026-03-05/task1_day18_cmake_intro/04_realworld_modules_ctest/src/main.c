#include <stdio.h>

#include "mathx.h"
#include "textx.h"

int main(void) {
    const int arr[] = {10, 20, 12};
    char out[32];

    printf("sum=%d\n", sum_array(arr, 3));
    to_upper_copy("cmake", out, (int)sizeof(out));
    printf("upper=%s\n", out);

    return 0;
}
