/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/color.h"
#include "fc/client/render/texture.h"
#include "fc/core/math/matrix.h"
#include "fc/core/math/vec2.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

class RenderBatcher
{
public:
    Matrix3x3 transform;

    virtual ~RenderBatcher() = default;

    struct Vertex
    {
        Vec2F pos;
        Vec2F textureCord;
        uint32_t color;
    };

    class Batchable
    {
    public:
        Texture* texture;

        Batchable(Texture* texture);
        [[nodiscard]] virtual size_t batchSize() const = 0;
        [[nodiscard]] virtual size_t indices() const = 0;

        virtual void addToBatcher(RenderBatcher* batcher) const = 0;
    };

    class TextureBatchable : public Batchable
    {
    public:
        Color tint;

        Matrix3x3 transform;

        Vec2U frameOffset;

        TextureBatchable(
            Texture* texture,
            const Color& tint,
            const Matrix3x3& transform,
            const Vec2U& frameOffset
        ) : Batchable(texture),
            tint(tint),
            transform(transform),
            frameOffset(frameOffset) { };

        [[nodiscard]] size_t batchSize() const override;
        [[nodiscard]] size_t indices() const override;

    private:
        void addToBatcher(RenderBatcher* batcher) const override;
    };

    virtual void addVertice(const Vertex& vert);
    virtual void addIndice(uint32_t i);

    [[nodiscard]] uint32_t indiceOffset() const
    {
        return m_indicesOffset;
    }

    void incrementIndiceOffset(uint32_t i)
    {
        m_indicesOffset += i;
    }

    virtual void addBatchable(const Batchable& batchable) = 0;

protected:
    static constexpr size_t MAX_BATCH_VERTICES = 4096;
    static constexpr size_t MAX_INDEX_SIZE = MAX_BATCH_VERTICES * 6;
    bool m_initialized = false;

    Vertex m_vertices[MAX_BATCH_VERTICES];
    uint32_t m_indices[MAX_INDEX_SIZE];

    size_t m_batchIndex = 0;
    size_t m_indicesIndex = 0;
    size_t m_indicesOffset = 0;
};
