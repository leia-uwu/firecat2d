/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/gl/glBatcher.h"
#include "fc/client/render/renderer.h"

class GlRenderer : public Renderer
{
public:
    GlRenderer(const InitFlags& flags) : Renderer(flags), m_batcher(std::make_unique<GlRenderBatcher>()) { };

    ~GlRenderer() override;

    SDL_AppResult init() override;

    RenderBatcher* batcher() override
    {
        return m_batcher.get();
    }

    void beginFrame() override;
    void endFrame() override;

    Texture* createTexture() override;

private:
    SDL_GLContext m_glContext;

    std::unique_ptr<GlRenderBatcher> m_batcher;

    void resize() override;
};
