#include "math_utils.h"

int add(int x, int y) {
    return x + y;
}

int max_of_two(int x, int y) {
    return (x > y) ? x : y;
}

int abs_diff(int x, int y) {
    int d = x - y;
    return (d < 0) ? -d : d;
}
