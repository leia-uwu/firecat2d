/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/app.h"

#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTextBlob.h>

SDL_AppResult GameApp::init(int /*argc*/, char** /*argv*/)
{
    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::update(float /*dt*/)
{
    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::SDLEvent(SDL_Event* /*event*/)
{
    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::processSDLIterate()
{
    m_ticker.frameStart();

    renderer().clear();

    float dt = m_ticker.deltaTime();
    SDL_AppResult result = update(dt);

    if (result != SDL_APP_CONTINUE) {
        return result;
    }

    inputManager().flush();

    m_root.updateTransform();

    Renderer::RenderCtx ctx{
        .renderer = &m_renderer,
        .canvas = m_renderer.surface()->getCanvas(),
        .dt = dt,
    };

    ctx.canvas->save();
    ctx.canvas->setMatrix(m_root.transform());
    m_root.renderChildren(ctx);
    ctx.canvas->restore();

    m_renderer.endFrame();

    m_ticker.frameEnd();

    SDL_GL_SwapWindow(m_renderer.window());

    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::processSDLEvent(SDL_Event* event)
{
    inputManager().processSDLEvent(event);
    renderer().processSDLEvent(event);

    SDL_AppResult result = SDLEvent(event);

    if (result != SDL_APP_CONTINUE) {
        return result;
    }

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
};
