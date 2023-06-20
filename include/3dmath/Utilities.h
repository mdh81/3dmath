#pragma once
#include "Constants.h"
namespace math3d {
    inline float asDegrees(float radians) {
        return radians * radiansToDegrees;
    };
}