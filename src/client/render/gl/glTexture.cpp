/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/gl/glTexture.h"

GlTexture::GlTexture() :
    internalFormat(GL_RGBA),
    imageFormat(GL_RGBA),
    wrapS(GL_REPEAT),
    wrapT(GL_REPEAT),
    filterMin(GL_NEAREST),
    filterMax(GL_NEAREST)
{
    glGenTextures(1, &id);
}

void GlTexture::generate(uint32_t width, uint32_t height, unsigned char* data)
{
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
    glGenerateMipmap(GL_TEXTURE_2D);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GlTexture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}

GlTexture::~GlTexture()
{
    glDeleteTextures(1, &id);
}
