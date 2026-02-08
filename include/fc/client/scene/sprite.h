/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/color.h"
#include "fc/client/render/batcher.h"
#include "fc/client/scene/container.h"

class SpriteItem : public Container
{
public:
    std::string textureId;

    uint32_t width;
    uint32_t height;

    Vec2U frameOffset;

    Vec2F center = {0.5, 0.5};

    Color tint = 0xffffff;

    float alpha = 1;

    void setTexture(const std::string& id)
    {
        textureId = id;
    }

    void render(const Matrix3x3& transform, Renderer* renderer) override
    {
        auto* texture = renderer->resources().getTexture(textureId);

        renderer->batcher()->addBatchable(RenderBatcher::TextureBatchable{
            texture,
            tint,
            Matrix3x3{transform}.mulScale({width / 2.F, height / 2.F}),
            frameOffset,
        });
    }
};
