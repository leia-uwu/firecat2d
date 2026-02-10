/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/core/math/vec2.h"

#include <array>
#include <cmath>
#include <cstdint>

// I HAVE NO IDEA WTH ANY OF THE MAGIC MATH HERE DOES!!!
class Matrix3x3
{
public:
    std::array<float, 9> arr;

    Matrix3x3() :
        arr({
            // clang-format off
              1, 0, 0,
              0, 1, 0,
              0, 0, 1
            // clang-format on
        })
    {
    }

    Matrix3x3(std::array<float, 9> data) : arr(data)
    {
    }

    Matrix3x3(const Vec2F& pos, float rot, const Vec2F& scale)
    {
        auto sin = std::sin(rot);
        auto cos = std::cos(rot);

        arr[0] = cos * scale.x;
        arr[1] = sin * scale.x;
        arr[2] = 0;
        arr[3] = -sin * scale.y;
        arr[4] = cos * scale.y;
        arr[5] = 0;
        arr[6] = pos.x;
        arr[7] = pos.y;
        arr[8] = 1;
    }

    [[nodiscard]] Vec2F translation() const
    {
        return {
            arr[6], arr[7]
        };
    }

    [[nodiscard]] float rotation() const
    {
        return std::atan2(arr[1], arr[0]);
    }

    [[nodiscard]] Vec2F scale() const
    {
        float x = Vec2(arr[0], arr[1]).length();
        float y = Vec2(arr[3], arr[4]).length();
        return {x, y};
    }

    Matrix3x3& mulScale(const Vec2F& scale)
    {
        arr[0] *= scale.x;
        arr[1] *= scale.x;

        arr[3] *= scale.y;
        arr[4] *= scale.y;

        return *this;
    };

    static Matrix3x3 mul(const Matrix3x3& a, const Matrix3x3& b)
    {
        Matrix3x3 out;
        out[0] = b[0] * a[0] + b[1] * a[3] + b[2] * a[6];
        out[1] = b[0] * a[1] + b[1] * a[4] + b[2] * a[7];
        out[2] = b[0] * a[2] + b[1] * a[5] + b[2] * a[8];
        out[3] = b[3] * a[0] + b[4] * a[3] + b[5] * a[6];
        out[4] = b[3] * a[1] + b[4] * a[4] + b[5] * a[7];
        out[5] = b[3] * a[2] + b[4] * a[5] + b[5] * a[8];
        out[6] = b[6] * a[0] + b[7] * a[3] + b[8] * a[6];
        out[7] = b[6] * a[1] + b[7] * a[4] + b[8] * a[7];
        out[8] = b[6] * a[2] + b[7] * a[5] + b[8] * a[8];

        return out;
    };

    [[nodiscard]] std::array<Vec2F, 4> getPolygon() const
    {
        Vec2F pos = {arr[6], arr[7]};
        Vec2F rotatedScaleX = {arr[0], arr[1]};
        Vec2F rotatedScaleY = {arr[3], arr[4]};

        return {
            (pos - (rotatedScaleX - rotatedScaleY)),
            (pos + (rotatedScaleX + rotatedScaleY)),
            (pos + (rotatedScaleX - rotatedScaleY)),
            (pos - (rotatedScaleX + rotatedScaleY)),
        };
    }

    [[nodiscard]] std::array<float, 12> to3x4() const
    {
        return {
            // clang-format off
            arr[0],arr[1],arr[2], 0.0,
            arr[3],arr[4],arr[5], 0.0,
            arr[6],arr[7],arr[8], 0.0,
            // clang-format on
        };
    }

    float operator[](uint8_t index) const
    {
        return arr[index];
    };

    float& operator[](uint8_t index)
    {
        return arr[index];
    };

    [[nodiscard]] std::string toString() const
    {
        return std::format(
            "{:.4f}, {:.4f}, {:.4f}\n{:.4f}, {:.4f}, {:.4f}\n{:.4f}, {:.4f}, {:.4f}\n",
            arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8]
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix3x3& vec)
    {
        os << vec.toString();
        return os;
    }
};
