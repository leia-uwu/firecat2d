/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/color.h"
#include "fc/client/scene/container.h"

#include <include/core/SkCanvas.h>
#include <include/core/SkColorFilter.h>

class SpriteItem : public Container
{
public:
    SpriteItem() : Container()
    {
    }
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

    SkPaint paint;

    void render(Renderer::RenderCtx& ctx) override
    {
        Texture* texture = ctx.renderer->resources().getTexture(textureId);

        auto color = SkColorSetARGB(alpha * 255, tint.r, tint.g, tint.b);
        paint.setColorFilter(SkColorFilters::Lighting(color, {}));

        float halfW = ((float)width) / 2.F;
        float halfH = ((float)height) / 2.F;

        SkRect rect{
            .fLeft = -halfW,
            .fTop = -halfH,
            .fRight = halfW,
            .fBottom = halfH,
        };

        ctx.canvas->drawImageRect(texture->image(), rect, texture->sampling, &paint);
    }
};
