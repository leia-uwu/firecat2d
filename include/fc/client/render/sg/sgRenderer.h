/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/renderer.h"
#include "fc/client/render/sg/sgBatcher.h"

#include <SDL3/SDL_gpu.h>

class SgRenderer : public Renderer
{
public:
    SgRenderer(const InitFlags& flags) : Renderer(flags), m_batcher(std::make_unique<SgRenderBatcher>()) { };

    ~SgRenderer() override;

    SDL_AppResult init() override;

    RenderBatcher* batcher() override
    {
        return m_batcher.get();
    }

    void beginFrame() override;
    void endFrame() override;

    Texture* createTexture() override;

private:
    SDL_GPUDevice* m_device;
    SDL_GPUCommandBuffer* m_cmdBuffer;
    SDL_GPURenderPass* m_renderPass;

    std::unique_ptr<SgRenderBatcher> m_batcher;

    void resize() override;
};
