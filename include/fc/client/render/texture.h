/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include <cstdint>

class Texture
{
public:
    uint32_t width;
    uint32_t height;

    Texture();
    Texture(const Texture& other) = delete;
    Texture operator=(const Texture& other) = delete;

    virtual void generate(uint32_t width, uint32_t height, uint8_t* data) = 0;

    void bind() const;

    virtual ~Texture() = default;
};
