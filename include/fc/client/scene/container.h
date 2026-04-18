/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/renderer.h"
#include "fc/core/math/vec2.h"

#include <SDL3/SDL_render.h>

#include <cassert>
#include <vector>

#include <include/core/SkM44.h>

class Container
{
protected:
    std::vector<Container*> m_children;
    Container* m_parent = nullptr;
    int32_t m_zIndex = 0;
    bool m_sortDirty = false;

    SkM44 m_transform;

public:
    Vec2F position;
    Vec2F scale = {1, 1};
    float rotation = 0;

    void updateTransform()
    {
        const float cos = std::cos(rotation);
        const float sin = std::sin(rotation);

        m_transform = {
            // clang-format off
             scale.x * cos,  scale.y * sin,   0,  position.x,
            -sin * scale.y,  cos * scale.y,   0,  position.y,
                         0,              0,   1,           0,
                         0,              0,   0,           1,
            // clang-format on
        };
    }

    [[nodiscard]] SkM44& transform()
    {
        return m_transform;
    }

    Container& addChild(Container* child);

    Container& addChild(std::initializer_list<Container*> children);

    Container& removeChild(Container* child);

    Container& clear();

    Container& setZIndex(int32_t idx);

    [[nodiscard]] int32_t zIndex() const
    {
        return m_zIndex;
    }

    void sortChildren();

    void renderChildren(Renderer::RenderCtx& ctx);

    virtual void render(Renderer::RenderCtx& ctx) { };

    virtual void destroyChildren();

    virtual ~Container();
};
