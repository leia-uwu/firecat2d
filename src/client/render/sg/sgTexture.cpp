/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/sg/sgTexture.h"
#include <SDL3/SDL_gpu.h>
#include <cstring>

SgTexture::SgTexture(SDL_GPUDevice* device) : m_device(device)
{
}

void SgTexture::generate(uint32_t width, uint32_t height, unsigned char* data)
{
    this->width = width;
    this->height = height;

    SDL_GPUTextureCreateInfo info{
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width = width,
        .height = height,
        .layer_count_or_depth = 1,
        .num_levels = (width == 1 || height == 1) ? 1U : 3U
    };
    m_texture = SDL_CreateGPUTexture(m_device, &info);

    uint32_t buffSize = width * height * 4;

    SDL_GPUTransferBufferCreateInfo buffInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = buffSize
    };

    auto* transferBuff = SDL_CreateGPUTransferBuffer(m_device, &buffInfo);

    void* mappedBuff = SDL_MapGPUTransferBuffer(m_device, transferBuff, false);

    memcpy(mappedBuff, data, buffSize);

    SDL_UnmapGPUTransferBuffer(m_device, transferBuff);

    auto* cmdBuff = SDL_AcquireGPUCommandBuffer(m_device);

    auto* copyPass = SDL_BeginGPUCopyPass(cmdBuff);

    SDL_GPUTextureTransferInfo transferInfo{
        .transfer_buffer = transferBuff,
        .offset = 0,
        .pixels_per_row = 0,
        .rows_per_layer = 0,
    };

    SDL_GPUTextureRegion region{
        .texture = m_texture,
        .mip_level = 0,
        .x = 0,
        .y = 0,
        .z = 0,
        .w = width,
        .h = height,
        .d = 1
    };

    SDL_UploadToGPUTexture(copyPass, &transferInfo, &region, false);
    SDL_EndGPUCopyPass(copyPass);

    if (width != 1 && height != 1) {
        SDL_GenerateMipmapsForGPUTexture(cmdBuff, m_texture);
    }

    SDL_SubmitGPUCommandBuffer(cmdBuff);
    SDL_ReleaseGPUTransferBuffer(m_device, transferBuff);

    SDL_GPUSamplerCreateInfo samplerInfo = {
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .mag_filter = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
    };
    m_sampler = SDL_CreateGPUSampler(m_device, &samplerInfo);
}

SgTexture::~SgTexture()
{
    if (m_texture != nullptr) {
        SDL_ReleaseGPUTexture(m_device, m_texture);
    }
    if (m_sampler != nullptr) {
        SDL_ReleaseGPUSampler(m_device, m_sampler);
    }
}
