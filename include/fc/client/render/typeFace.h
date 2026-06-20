/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/core/buffer.h"
#include <include/core/SkTypeface.h>

/**
 * Wraps around SkTypeFace to be lazy loaded
 */
class TypeFace
{
public:
    TypeFace() = default;
    TypeFace(const TypeFace& other) = delete;
    TypeFace operator=(const TypeFace& other) = delete;

    void load(const Uint8Buffer& data);

    sk_sp<SkTypeface> typeFace()
    {
        if (m_loaded) {
            return m_typeFace;
        }
        return SkTypeface::MakeEmpty();
    }

private:
    bool m_loaded = false;
    sk_sp<SkTypeface> m_typeFace;
};
