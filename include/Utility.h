#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

inline double randomDouble() {
    return random() / (RAND_MAX + 1.0); // [0,1)
}

inline double randomDouble(double min, double max) {
    return min + (max-min)*randomDouble();
}

inline double clamp(double x, double min, double max) {
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

#include "Ray.h"
#include "Vector3.h"

#endif