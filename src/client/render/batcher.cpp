/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/batcher.h"

#include <cstdint>

void RenderBatcher::addVertice(const Vertex& vert)
{
    assert((m_batchIndex + 1) < MAX_BATCH_VERTICES);

    m_vertices[m_batchIndex] = vert;
    m_batchIndex++;
}

void RenderBatcher::addIndice(uint32_t i)
{
    assert((m_indicesIndex + 1) < MAX_INDEX_SIZE);

    m_indices[m_indicesIndex] = i;
    m_indicesIndex++;
}

RenderBatcher::Batchable::Batchable(Texture* texture) : texture(texture)
{
}

[[nodiscard]] size_t RenderBatcher::TextureBatchable::batchSize() const
{
    return 4;
}

[[nodiscard]] size_t RenderBatcher::TextureBatchable::indices() const
{
    return 6;
}

void RenderBatcher::TextureBatchable::addToBatcher(RenderBatcher* batcher) const
{
    uint32_t color = tint.RGBAHex();

    auto poly = transform.getPolygon();

    batcher->addVertice({
        .pos = poly[0],
        .textureCord = {0.F, 0.F},
        .color = color,
    });

    batcher->addVertice({
        .pos = poly[1],
        .textureCord = {1.F, 0.F},
        .color = color,
    });

    batcher->addVertice({
        .pos = poly[2],
        .textureCord = {1.F, 1.F},
        .color = color,
    });

    batcher->addVertice({
        .pos = poly[3],
        .textureCord = {0.F, 1.F},
        .color = color,
    });

    size_t offset = batcher->indiceOffset();

    batcher->addIndice(0 + offset);
    batcher->addIndice(1 + offset);
    batcher->addIndice(2 + offset);

    batcher->addIndice(2 + offset);
    batcher->addIndice(3 + offset);
    batcher->addIndice(0 + offset);

    batcher->incrementIndiceOffset(4);
}
