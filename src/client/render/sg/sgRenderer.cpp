/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/sg/sgRenderer.h"
#include "fc/client/render/sg/sgTexture.h"

#include <iostream>

#include <SDL3/SDL_video.h>

SgRenderer::~SgRenderer()
{
    SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
    SDL_DestroyWindow(m_window);
    SDL_DestroyGPUDevice(m_device);
}

SDL_AppResult SgRenderer::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    };

    m_device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        false,
        nullptr
    );

    if (m_device == nullptr) {
        std::cerr << "SDL_CreateGPUDevice error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    SDL_WindowFlags flags = 0;

    if (m_resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    m_window = SDL_CreateWindow(
        m_windowTitle.c_str(),
        m_windowWidth,
        m_windowHeight,
        flags
    );

    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    if (!SDL_ClaimWindowForGPUDevice(m_device, m_window)) {
        std::cerr << "SDL_ClaimWindowForGPUDevice error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    m_batcher->init(m_device);

    resize();

    return SDL_APP_CONTINUE;
}

void SgRenderer::beginFrame()
{
    m_cmdBuffer = SDL_AcquireGPUCommandBuffer(m_device);

    SDL_GPUTexture* swapchainTexture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(m_cmdBuffer, m_window, &swapchainTexture, nullptr, nullptr)) {
        std::cerr << "SDL_WaitAndAcquireGPUSwapchainTexture error: " << SDL_GetError() << "\n";
        return;
    }

    SDL_GPUColorTargetInfo colorTargetInfo = {};
    colorTargetInfo.texture = swapchainTexture;

    colorTargetInfo.clear_color = (SDL_FColor){
        m_clearColor.normalizedR(),
        m_clearColor.normalizedG(),
        m_clearColor.normalizedB(),
        m_clearColor.normalizedA()
    };

    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

    m_renderPass = SDL_BeginGPURenderPass(m_cmdBuffer, &colorTargetInfo, 1, nullptr);

    SgTexture* tex = static_cast<SgTexture*>(m_resources.getTexture("bird"));

    uint32_t miplevel = 0;

    SDL_GPUBlitInfo info = {
        .source = {
            .texture = tex->m_texture,
            .mip_level = miplevel,
            .layer_or_depth_plane = 0,
            .x = 0,
            .y = 0,
            .w = tex->width / (1 << miplevel),
            .h = tex->height / (1 << miplevel),
        },
        .destination = {
            .texture = swapchainTexture,
            .mip_level = 0,
            .layer_or_depth_plane = 0,
            .x = 64,
            .y = 64,
            .w = tex->width / (1 << miplevel),
            .h = tex->height / (1 << miplevel),
        },
        .load_op = SDL_GPU_LOADOP_LOAD,
        .clear_color = {1, 0, 0, 1},
        .flip_mode = SDL_FLIP_NONE,
        .filter = SDL_GPU_FILTER_LINEAR,
        .cycle = false,
    };
    SDL_BlitGPUTexture(m_cmdBuffer, &info);

    m_batcher->beginBatch();
}

void SgRenderer::endFrame()
{
    m_batcher->flushBatch();
    // SDL_EndGPURenderPass(m_renderPass);
    m_renderPass = nullptr;
    SDL_SubmitGPUCommandBuffer(m_cmdBuffer);
    m_cmdBuffer = nullptr;
}

void SgRenderer::resize()
{
    m_batcher->transform = Matrix3x3(
        {0, 0},
        0,
        {1.F / (m_windowWidth / 2.F), (1.F / (m_windowHeight / 2.F))}
    );
};
Texture* SgRenderer::createTexture()
{
    return new SgTexture(m_device);
}
