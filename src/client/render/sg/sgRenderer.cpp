/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/sg/sgRenderer.h"
#include "fc/client/render/sg/sgTexture.h"

#include <SDL3/SDL_gpu.h>
#include <iostream>

#include <SDL3/SDL_video.h>

SgRenderer::~SgRenderer()
{
    delete m_resources;
    delete m_batcher;
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
        true,
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

    SDL_SetGPUSwapchainParameters(m_device, m_window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

    std::cout << "Using SDL_GPU with " << SDL_GetGPUDeviceDriver(m_device) << "\n";

    m_batcher->init(m_device, m_window);

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
        .r = m_clearColor.normalizedR(),
        .g = m_clearColor.normalizedG(),
        .b = m_clearColor.normalizedB(),
        .a = m_clearColor.normalizedA()
    };

    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

    m_renderPass = SDL_BeginGPURenderPass(m_cmdBuffer, &colorTargetInfo, 1, nullptr);

    auto t = m_batcher->transform.to3x4();
    SDL_PushGPUVertexUniformData(m_cmdBuffer, 1, &t, sizeof(t));

    m_batcher->beginBatch(m_renderPass);
}

void SgRenderer::endFrame()
{
    m_batcher->flushBatch();
    SDL_EndGPURenderPass(m_renderPass);
    m_renderPass = nullptr;
    SDL_SubmitGPUCommandBuffer(m_cmdBuffer);
    m_cmdBuffer = nullptr;
}

void SgRenderer::resize()
{
    m_batcher->transform = Matrix3x3(
        {-1, -1},
        0,
        {1.F / (m_windowWidth / 2.F), (1.F / (m_windowHeight / 2.F))}
    );
};
Texture* SgRenderer::createTexture()
{
    return new SgTexture(m_device);
}
