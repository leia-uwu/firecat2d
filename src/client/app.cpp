/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/app.h"

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

    renderer()->beginFrame();

    SDL_AppResult result = update(m_ticker.deltaTime());

    if (result != SDL_APP_CONTINUE) {
        return result;
    }

    inputManager().flush();

    m_renderer->endFrame();

    m_ticker.frameEnd();

    return SDL_APP_CONTINUE;
};

SDL_AppResult GameApp::processSDLEvent(SDL_Event* event)
{
    inputManager().processSDLEvent(event);
    renderer()->processSDLEvent(event);

    SDL_AppResult result = SDLEvent(event);

    if (result != SDL_APP_CONTINUE) {
        return result;
    }

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
};
