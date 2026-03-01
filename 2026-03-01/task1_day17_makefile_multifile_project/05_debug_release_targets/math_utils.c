#include "math_utils.h"

int sum_array(const int *arr, int n) {
    int sum = 0;
    int i;

    for (i = 0; i < n; ++i) {
        sum += arr[i];
    }

    return sum;
}

int max_array(const int *arr, int n) {
    int i;
    int m = arr[0];

    for (i = 1; i < n; ++i) {
        if (arr[i] > m) {
            m = arr[i];
        }
    }

    return m;
}
