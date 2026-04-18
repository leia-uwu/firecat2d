/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/texture.h"

#include <cstddef>

#include <include/core/SkColorSpace.h>
#include <include/core/SkData.h>

Texture::Texture() :
    width(0),
    height(0)
{
}

void Texture::generate(uint32_t width, uint32_t height, unsigned char* data)
{
    this->width = width;
    this->height = height;

    auto info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
    auto skData = SkData::MakeFromMalloc(data, (static_cast<size_t>(width * height)) * 4);

    m_image = SkImages::RasterFromData(
        info,
        skData,
        info.minRowBytes()
    );
}
