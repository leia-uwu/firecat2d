/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/scene/sprite.h"
#include "fc/client/resources.h"

void SpriteItem::render(Renderer::RenderCtx& ctx)
{
    Texture* texture = Resources::get().getTexture(textureId);

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
