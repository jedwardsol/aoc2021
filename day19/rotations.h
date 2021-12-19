#pragma once
#include "rotation.h"

constexpr Rotation XAxis
{{
    { 1, 0, 0},
    { 0, 0,-1},
    { 0, 1, 0},
}};

constexpr Rotation YAxis
{{
    { 0, 0, 1},
    { 0, 1, 0},
    {-1, 0, 0},
}};

constexpr Rotation ZAxis
{{
    { 0,-1, 0},
    { 1, 0, 0},
    { 0, 0, 1},
}};
