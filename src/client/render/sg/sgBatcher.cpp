/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/sg/sgBatcher.h"
#include "fc/client/render/sg/sgTexture.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>
#include <cstdint>
#include <iostream>

void SgRenderBatcher::init(SDL_GPUDevice* device)
{
    assert(m_initialized == false);
    m_initialized = true;

    m_device = device;

    SDL_GPUBufferCreateInfo info{};

    info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    info.size = sizeof(m_vertices);
    m_vertexBuffer = SDL_CreateGPUBuffer(m_device, &info);
    if (m_vertexBuffer == nullptr) {
        std::cerr << "SDL_CreateGPUBuffer error: " << SDL_GetError() << "\n";
    }

    info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    info.size = sizeof(m_indices);
    m_indexBuffer = SDL_CreateGPUBuffer(m_device, &info);
    if (m_vertexBuffer == nullptr) {
        std::cerr << "SDL_CreateGPUBuffer error: " << SDL_GetError() << "\n";
    }
}

SgRenderBatcher::~SgRenderBatcher()
{
    // releasing the buffers somehow locks the main thread?????????
    // since this is called when the app exits anyway its not really a memory leak
    // TODO: figure out why it freezes the main thread
    // SDL_ReleaseGPUBuffer(m_device, m_vertexBuffer);
    // SDL_ReleaseGPUBuffer(m_device, m_indexBuffer);
}

void SgRenderBatcher::addVertice(const Vertex& vert)
{
    assert((m_batchIndex + 1) < MAX_BATCH_VERTICES);

    m_vertices[m_batchIndex] = vert;
    m_batchIndex++;
}

void SgRenderBatcher::addIndice(uint32_t i)
{
    assert((m_indicesIndex + 1) < MAX_INDEX_SIZE);

    m_indices[m_indicesIndex] = i;
    m_indicesIndex++;
}

void SgRenderBatcher::beginBatch()
{
}

void SgRenderBatcher::flushBatch()
{
}

void SgRenderBatcher::addBatchable(const Batchable& batchable)
{
}
