/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/inputManager.h"
#include "fc/client/render/gl/glRenderer.h"
#include "fc/client/render/renderer.h"
#include "fc/client/render/sg/sgRenderer.h"
#include "fc/core/ticker.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>

class GameApp
{
public:
    GameApp(const Renderer::InitFlags& flags)
    {
        m_renderer = new SgRenderer(flags);
    }
    virtual SDL_AppResult init(int argc, char** argv);

    /**
     * User provided update function
     * dt is the "delta time" / time in seconds since the last update.
     */
    virtual SDL_AppResult update(float dt);

    /**
     * User provided callback for SDL events
     */
    virtual SDL_AppResult SDLEvent(SDL_Event* event);

    virtual ~GameApp()
    {
        delete m_renderer;
    };

    Renderer* renderer()
    {
        return m_renderer;
    }

    InputManager& inputManager()
    {
        return m_inputManager;
    }

    Ticker& ticker()
    {
        return m_ticker;
    }

    SDL_AppResult processSDLIterate();

    SDL_AppResult processSDLEvent(SDL_Event* event);

protected:
    Renderer* m_renderer;

    InputManager m_inputManager;

    Ticker m_ticker;
};

// NOLINTBEGIN(bugprone-macro-parentheses)

/**
 * Initializes the application, APP must be a valid subclass of GameApp
 * This will:
 * - call app->renderer().init() then app->init(), exiting if any of them fail
 * - Set up SDL event handlers (SDL_AppIterate, SDL_AppEvent and SDL_AppQuit)
 */
#define INIT_APP(APP)                                                 \
    SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) \
    {                                                                 \
        auto* app = new APP();                                        \
                                                                      \
        SDL_AppResult result = app->renderer()->init();               \
        if (result != SDL_APP_CONTINUE) {                             \
            return result;                                            \
        }                                                             \
                                                                      \
        result = app->init(argc, argv);                               \
        if (result != SDL_APP_CONTINUE) {                             \
            return result;                                            \
        }                                                             \
                                                                      \
        *appstate = app;                                              \
        return SDL_APP_CONTINUE;                                      \
    };                                                                \
                                                                      \
    SDL_AppResult SDL_AppIterate(void* appstate)                      \
    {                                                                 \
        auto* app = static_cast<APP*>(appstate);                      \
        return app->processSDLIterate();                              \
    };                                                                \
                                                                      \
    SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)      \
    {                                                                 \
        auto* app = static_cast<APP*>(appstate);                      \
        return app->processSDLEvent(event);                           \
    };                                                                \
                                                                      \
    void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/)        \
    {                                                                 \
        auto* app = static_cast<APP*>(appstate);                      \
        delete app;                                                   \
    };

// NOLINTEND(bugprone-macro-parentheses)
