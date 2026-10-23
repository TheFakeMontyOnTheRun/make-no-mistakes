#include "util.h"

#include <math.h>

void normalize_vector(float *x, float *y) {
    if (!x || !y) {
        return;
    }

    float magnitude = sqrtf((*x * *x) + (*y * *y));
    if (magnitude > 0.0f) {
        *x /= magnitude;
        *y /= magnitude;
    }
}

