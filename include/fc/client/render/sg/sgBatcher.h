/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/batcher.h"

#include <SDL3/SDL_gpu.h>

class SgRenderBatcher : public RenderBatcher
{
public:
    SgRenderBatcher() : RenderBatcher()
    {
    }

    void init(SDL_GPUDevice* device);
    ~SgRenderBatcher() override;

    void addVertice(const Vertex& vert) override;
    void addIndice(uint32_t i) override;

    void addBatchable(const Batchable& batchable) override;

    void beginBatch();
    void flushBatch();

private:
    SDL_GPUDevice* m_device;

    SDL_GPUBuffer* m_vertexBuffer;
    SDL_GPUBuffer* m_indexBuffer;
};
