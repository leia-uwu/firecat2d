/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/renderer.h"
#include "fc/core/math/matrix.h"
#include "fc/core/math/vec2.h"

#include <glad/gl.h>

#include <SDL3/SDL_render.h>

#include <cassert>
#include <vector>

class Container
{
private:
    std::vector<Container*> m_children;
    Container* m_parent = nullptr;
    int32_t m_zIndex = 0;
    bool m_sortDirty = false;

public:
    Vec2F position;
    Vec2F scale = {1, 1};
    float rotation = 0;

    [[nodiscard]] Matrix3x3 getMatrix() const
    {
        return {position, rotation, scale};
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

    void renderChildren(const Matrix3x3& transform, Renderer* renderer);

    virtual void render(const Matrix3x3& transform, Renderer* renderer) { };

    virtual void destroyChildren();

    virtual ~Container();
};
