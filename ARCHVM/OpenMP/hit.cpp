#include <cmath>
#include "hit.h"

bool hit_test(float x, float y, float z) {
    return (x*x*x*x - 2 * x*x*x) + 4*(y*y + z*z) <= 0;
}

const float* get_axis_range() {
    float* range = new float[6];

    range[0] = 0;
    range[1] = 2;
    range[2] = -0.6495;
    range[3] = 0.6495;
    range[4] = -0.6495;
    range[5] = 0.6495;

    return range;
}