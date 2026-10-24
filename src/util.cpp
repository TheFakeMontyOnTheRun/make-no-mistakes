#include "util.h"

#include <math.h>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

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

int randomNumber(const int min, const int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}
