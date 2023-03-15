#pragma once

#include <cmath>

constexpr static double pi = 3.14159;
constexpr static double seconds_per_frame = 0.015;
constexpr static int no_stage = -1;
constexpr static int no_object = 0;

//计算距离
inline double distance(double x1, double y1, double x2, double y2) {
    return pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
}
