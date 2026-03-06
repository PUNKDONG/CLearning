#include <string.h>

#include "textx.h"

int main(void) {
    char out[16];

    to_upper_copy("abC1", out, (int)sizeof(out));
    if (strcmp(out, "ABC1") != 0) {
        return 1;
    }

    to_upper_copy("cmake", out, 4);
    if (strcmp(out, "CMA") != 0) {
        return 2;
    }

    return 0;
}
