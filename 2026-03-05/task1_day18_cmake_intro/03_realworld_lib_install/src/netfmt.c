#include <stdio.h>

#include "netfmt.h"

const char *build_endpoint(const char *scheme, const char *host, int port, const char *path) {
    static char buf[256];
    snprintf(buf, sizeof(buf), "%s://%s:%d%s", scheme, host, port, path);
    return buf;
}
