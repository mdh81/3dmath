#pragma once

#include <numbers>

// List of constants used in this library
// NOTE: Don't include any constant that's dependent on this library's data-structures
// to avoid circular dependencies
namespace math3d::constants {
    constexpr float threeSixtyDegrees = 2 * std::numbers::pi;
    constexpr float oneEightyDegrees = std::numbers::pi;
    constexpr float radiansToDegrees = 180 / std::numbers::pi;
    constexpr float tolerance = 1e-6;
    constexpr std::initializer_list<float> xAxis = {1.f, 0.f, 0.f};
    constexpr std::initializer_list<float> yAxis = {0.f, 1.f, 0.f};
    constexpr std::initializer_list<float> zAxis = {0.f, 0.f, 1.f};
    constexpr std::initializer_list<float> origin = {0.f, 0.f, 0.f};
}