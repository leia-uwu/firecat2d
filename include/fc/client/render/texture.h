/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include <cstdint>
#include <include/core/SkImage.h>
#include <include/core/SkRefCnt.h>
#include <include/core/SkSamplingOptions.h>

class SkImage;

class Texture
{
public:
    uint32_t width;
    uint32_t height;

    Texture();
    Texture(const Texture& other) = delete;
    Texture operator=(const Texture& other) = delete;

    void generate(uint32_t width, uint32_t height, uint8_t* data);

    sk_sp<SkImage> image()
    {
        return m_image;
    }

    SkSamplingOptions sampling;

private:
    sk_sp<SkImage> m_image;
};
