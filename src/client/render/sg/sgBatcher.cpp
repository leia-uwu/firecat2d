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

#include "../shaders/sprite.frag.spv.h"
#include "../shaders/sprite.vert.spv.h"

void SgRenderBatcher::init(SDL_GPUDevice* device, SDL_Window* window)
{
    assert(m_initialized == false);
    m_initialized = true;

    m_device = device;

    SDL_GPUBufferCreateInfo info{};

    info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    info.size = VERTEX_BUFFER_SIZE;
    m_vertexBuffer = SDL_CreateGPUBuffer(m_device, &info);
    if (m_vertexBuffer == nullptr) {
        std::cerr << "SDL_CreateGPUBuffer error: " << SDL_GetError() << "\n";
    }

    info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    info.size = INDEX_BUFFER_SIZE;
    m_indexBuffer = SDL_CreateGPUBuffer(m_device, &info);
    if (m_vertexBuffer == nullptr) {
        std::cerr << "SDL_CreateGPUBuffer error: " << SDL_GetError() << "\n";
    }

    SDL_GPUTransferBufferCreateInfo buffInf{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = VERTEX_BUFFER_SIZE,
    };

    m_vertexTransferBuffer = SDL_CreateGPUTransferBuffer(
        m_device,
        &buffInf
    );
    buffInf.size = INDEX_BUFFER_SIZE;
    m_indexTransferBuffer = SDL_CreateGPUTransferBuffer(
        m_device,
        &buffInf
    );

    SDL_GPUShaderCreateInfo fragShaderInfo{
        .code_size = sizeof(sprite_frag),
        .code = (uint8_t*)&sprite_frag,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
        .num_samplers = 1,
        .num_uniform_buffers = 0
    };

    SDL_GPUShader* fragShader = SDL_CreateGPUShader(m_device, &fragShaderInfo);

    if (fragShader == nullptr) {
        std::cerr << "SDL_CreateGPUShader error: " << SDL_GetError() << "\n";
    }

    SDL_GPUShaderCreateInfo vertShaderInfo{
        .code_size = sizeof(sprite_vert),
        .code = (uint8_t*)&sprite_vert,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = SDL_GPU_SHADERSTAGE_VERTEX,
        .num_uniform_buffers = 1,
    };

    SDL_GPUShader* vertShader = SDL_CreateGPUShader(m_device, &vertShaderInfo);

    if (vertShader == nullptr) {
        std::cerr << "SDL_CreateGPUShader error: " << SDL_GetError() << "\n";
    }

    // Create the pipeline
    SDL_GPUVertexBufferDescription desc = {
        .slot = 0,
        .pitch = sizeof(Vertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
    };

    SDL_GPUVertexAttribute attr[]{
        {
            .location = 0,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
            .offset = 0,
        },
        {
            .location = 1,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
            .offset = 8,
        },
        {
            .location = 2,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_UINT,
            .offset = 16,
        },
    };

    SDL_GPUVertexInputState inputState = {
        .vertex_buffer_descriptions = &desc,
        .num_vertex_buffers = 1,
        .vertex_attributes = attr,
        .num_vertex_attributes = 3,
    };

    SDL_GPUColorTargetDescription colorDesc{
        .format = SDL_GetGPUSwapchainTextureFormat(m_device, window),
    };

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .vertex_shader = vertShader,
        .fragment_shader = fragShader,
        .vertex_input_state = inputState,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = {
            .color_target_descriptions = &colorDesc,
            .num_color_targets = 1,
        },
    };

    m_pipeline = SDL_CreateGPUGraphicsPipeline(m_device, &pipelineCreateInfo);
    if (m_pipeline == nullptr) {
        std::cerr << "SDL_CreateGPUGraphicsPipeline error: " << SDL_GetError() << "\n";
    }

    SDL_ReleaseGPUShader(m_device, vertShader);
    SDL_ReleaseGPUShader(m_device, fragShader);

    m_whiteTexture = std::make_unique<SgTexture>(m_device);

    uint8_t white[]{255, 255, 255, 255};
    m_whiteTexture->generate(1, 1, white);

    m_lastTexture = m_whiteTexture.get();
}

SgRenderBatcher::~SgRenderBatcher()
{
    SDL_ReleaseGPUBuffer(m_device, m_vertexBuffer);
    SDL_ReleaseGPUBuffer(m_device, m_indexBuffer);
    SDL_ReleaseGPUGraphicsPipeline(m_device, m_pipeline);

    SDL_ReleaseGPUTransferBuffer(m_device, m_vertexTransferBuffer);
    SDL_ReleaseGPUTransferBuffer(m_device, m_indexTransferBuffer);
}

void SgRenderBatcher::addVertice(const Vertex& vert)
{
    assert((m_batchIndex + 1) < MAX_BATCH_VERTICES);

    m_mappedVertices[m_batchIndex] = vert;
    m_batchIndex++;
}

void SgRenderBatcher::addIndice(uint32_t i)
{
    assert((m_indicesIndex + 1) < MAX_INDEX_SIZE);

    m_mappedIndices[m_indicesIndex] = i;
    m_indicesIndex++;
}

void SgRenderBatcher::beginBatch(SDL_GPURenderPass* renderPass)
{
    m_renderPass = renderPass;
    m_batchIndex = 0;
    m_indicesIndex = 0;
    m_indicesOffset = 0;

    m_currentTexture = nullptr;
    m_lastTexture = nullptr;

    m_mappedVertices = (Vertex*)SDL_MapGPUTransferBuffer(
        m_device,
        m_vertexTransferBuffer,
        true
    );
    m_mappedIndices = (uint16_t*)SDL_MapGPUTransferBuffer(
        m_device,
        m_indexTransferBuffer,
        true
    );
}

void SgRenderBatcher::flushBatch(SgTexture* texture)
{
    if (m_batchIndex == 0 || m_indicesIndex == 0)
        return;

    if (texture == nullptr) {
        texture = m_lastTexture;
    }

    uint32_t vertSize = m_batchIndex * sizeof(Vertex);
    uint32_t indexSize = m_indicesIndex * sizeof(uint16_t);

    SDL_UnmapGPUTransferBuffer(m_device, m_vertexTransferBuffer);
    SDL_UnmapGPUTransferBuffer(m_device, m_indexTransferBuffer);

    SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(m_device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

    SDL_GPUTransferBufferLocation buffLoc{
        .transfer_buffer = m_vertexTransferBuffer,
        .offset = 0
    };
    SDL_GPUBufferRegion buffReg{
        .buffer = m_vertexBuffer,
        .offset = 0,
        .size = vertSize
    };
    SDL_UploadToGPUBuffer(
        copyPass,
        &buffLoc,
        &buffReg,
        true
    );

    SDL_GPUTransferBufferLocation buffLoc2{
        .transfer_buffer = m_indexTransferBuffer,
        .offset = 0
    };
    SDL_GPUBufferRegion buffReg2{
        .buffer = m_indexBuffer,
        .offset = 0,
        .size = indexSize
    };

    SDL_UploadToGPUBuffer(
        copyPass,
        &buffLoc2,
        &buffReg2,
        true
    );

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(uploadCmdBuf);

    SDL_BindGPUGraphicsPipeline(m_renderPass, m_pipeline);
    SDL_GPUBufferBinding bind{
        .buffer = m_vertexBuffer,
        .offset = 0,
    };
    SDL_BindGPUVertexBuffers(m_renderPass, 0, &bind, 1);
    SDL_GPUBufferBinding bind2{
        .buffer = m_indexBuffer,
        .offset = 0,
    };
    SDL_BindGPUIndexBuffer(m_renderPass, &bind2, SDL_GPU_INDEXELEMENTSIZE_16BIT);

    SDL_BindGPUFragmentSamplers(
        m_renderPass,
        0,
        (SDL_GPUTextureSamplerBinding[]){
            {.texture = texture->m_texture, .sampler = texture->m_sampler},
        },
        1
    );

    SDL_DrawGPUIndexedPrimitives(m_renderPass, m_indicesIndex, 1, 0, 0, 0);
}

void SgRenderBatcher::addBatchable(const Batchable& batchable)
{
    size_t oldBatchIndex = m_batchIndex;
    size_t oldIndiceIndex = m_indicesIndex;

    // std::cout << batchable.texture << "\n";

    m_currentTexture = static_cast<SgTexture*>(batchable.texture);

    if (m_batchIndex + batchable.batchSize() >= MAX_BATCH_VERTICES || (m_lastTexture != nullptr && m_lastTexture != m_currentTexture)) {
        // std::cout << "FLUSH\n";
        oldBatchIndex = 0;
        oldIndiceIndex = 0;
        flushBatch(m_lastTexture);
        beginBatch(m_renderPass);
        m_currentTexture = static_cast<SgTexture*>(batchable.texture);
    }
    m_lastTexture = m_currentTexture;

    batchable.addToBatcher(this);

    // to make sure addToBatcher and batchSize are in sync
    assert(m_batchIndex == (oldBatchIndex + batchable.batchSize()));
    assert(m_indicesIndex == (oldIndiceIndex + batchable.indices()));
}
