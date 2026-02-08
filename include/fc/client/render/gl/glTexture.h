/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/texture.h"
#include <glad/gl.h>

#include <cstdint>

class GlTexture : public Texture
{
public:
    GLuint id;

    GLuint internalFormat;
    GLuint imageFormat;

    GLuint wrapS;
    GLuint wrapT;
    GLuint filterMin;
    GLuint filterMax;

    GlTexture();
    GlTexture(const GlTexture& other) = delete;
    GlTexture operator=(const GlTexture& other) = delete;

    void generate(uint32_t width, uint32_t height, uint8_t* data) override;

    void bind() const;

    ~GlTexture() override;
};
