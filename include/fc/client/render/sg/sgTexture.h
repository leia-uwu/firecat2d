/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/texture.h"

#include <cstdint>

#include <SDL3/SDL_gpu.h>

class SgTexture : public Texture
{
public:
    SgTexture(SDL_GPUDevice* device);
    SgTexture(const SgTexture& other) = delete;
    SgTexture operator=(const SgTexture& other) = delete;

    void generate(uint32_t width, uint32_t height, uint8_t* data) override;

    void bind() const;

    ~SgTexture() override;

    SDL_GPUTexture* m_texture = nullptr;
private:
    SDL_GPUDevice* m_device;
};
