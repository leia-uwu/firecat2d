/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/batcher.h"
#include "fc/client/render/sg/sgTexture.h"

#include <SDL3/SDL_gpu.h>
#include <memory>

class SgRenderBatcher : public RenderBatcher
{
public:
    SgRenderBatcher() : RenderBatcher()
    {
    }

    void init(SDL_GPUDevice* device, SDL_Window* window);
    ~SgRenderBatcher() override;

    void addVertice(const Vertex& vert) override;
    void addIndice(uint32_t i) override;

    void addBatchable(const Batchable& batchable) override;

    void beginBatch(SDL_GPURenderPass*);
    void flushBatch(SgTexture* texture = nullptr);

private:
    SDL_GPUDevice* m_device;

    SDL_GPUGraphicsPipeline* m_pipeline;

    std::unique_ptr<SgTexture> m_whiteTexture;
    SgTexture* m_lastTexture;
    SgTexture* m_currentTexture;

    SDL_GPUBuffer* m_vertexBuffer;
    SDL_GPUTransferBuffer* m_vertexTransferBuffer;
    Vertex* m_mappedVertices;

    SDL_GPUBuffer* m_indexBuffer;
    SDL_GPUTransferBuffer* m_indexTransferBuffer;
    uint16_t* m_mappedIndices;

    SDL_GPURenderPass* m_renderPass;
};
