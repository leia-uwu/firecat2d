/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/batcher.h"
#include "fc/client/render/gl/glTexture.h"
#include "fc/client/render/gl/shader.h"

#include <cassert>
#include <memory>

class GlRenderBatcher : public RenderBatcher
{
public:
    GlRenderBatcher() : RenderBatcher()
    {
    }

    void init();
    ~GlRenderBatcher() override;

    void addVertice(const Vertex& vert) override;
    void addIndice(uint32_t i) override;

    void addBatchable(const Batchable& batchable) override;

    void beginBatch();
    void flushBatch();

private:
    Shader m_spriteShader;

    Vertex m_vertices[MAX_BATCH_VERTICES];
    uint16_t m_indices[MAX_INDEX_SIZE];

    GLuint m_quadVAO;
    GLuint m_quadVBO;
    GLuint m_quadEBO;

    GlTexture* m_lastTexture;
    std::unique_ptr<GlTexture> m_whiteTexture;
};
