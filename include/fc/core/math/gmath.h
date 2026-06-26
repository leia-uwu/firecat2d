/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include <cstdlib>
#include <numbers>

// you might think this means "Game Math" since this is inside a game engine code
//
// WRONG!
// Gay Math.

namespace GMath
{

template<typename T>
inline int Sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

inline bool EqAbs(double a, double b, double eps = 0.000001)
{
    return std::abs(a - b) < eps;
}

inline bool EqAbs(float a, float b, float eps = 0.000001)
{
    return std::abs(a - b) < eps;
}
// MSVC is stupid and doesn't have this on cmath?????
inline constexpr double PI_2 = std::numbers::pi / 2.0;
};
