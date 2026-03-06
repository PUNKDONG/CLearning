#include <stdio.h>

#include "netfmt.h"

int main(void) {
    puts(build_endpoint("https", "api.example.com", 8443, "/v1/ping"));
    return 0;
}
