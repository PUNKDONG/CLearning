#include "math_utils.h"

int add(int x, int y) {
    return x + y;
}

int abs_diff(int x, int y) {
    int d = x - y;
    return (d < 0) ? -d : d;
}

int gcd(int x, int y) {
    while (y != 0) {
        int t = x % y;
        x = y;
        y = t;
    }

    return (x < 0) ? -x : x;
}
