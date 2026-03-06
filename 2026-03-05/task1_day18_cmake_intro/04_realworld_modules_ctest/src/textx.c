#include <ctype.h>

#include "textx.h"

void to_upper_copy(const char *in, char *out, int cap) {
    int i = 0;
    if (cap <= 0) {
        return;
    }
    while (in[i] != '\0' && i < cap - 1) {
        out[i] = (char)toupper((unsigned char)in[i]);
        ++i;
    }
    out[i] = '\0';
}
