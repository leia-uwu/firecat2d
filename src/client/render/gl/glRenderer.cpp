/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/gl/glRenderer.h"

#include <iostream>

#include <SDL3/SDL_video.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

EM_JS(int, getWindowWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getWindowHeight, (), {
    return window.innerHeight;
});

static bool resizeCanvas(int eventType, const EmscriptenUiEvent* event, void* userData)
{
    auto* renderer = static_cast<Renderer*>(userData);
    if (!renderer->resizable())
        return 0;

    int width = getWindowWidth();
    int height = getWindowHeight();

    if (!SDL_SetWindowSize(renderer->window(), width, height)) {
        std::cout << "SDL_SetWindowSize error: " << SDL_GetError() << "\n";
    }
    return 0;
};
#endif

GlRenderer::~GlRenderer()
{
    SDL_GL_DestroyContext(m_glContext);
    SDL_DestroyWindow(m_window);
}

SDL_AppResult GlRenderer::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    };

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_WindowFlags flags = SDL_WINDOW_OPENGL;

    if (m_resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    m_window = SDL_CreateWindow(
        m_windowTitle.c_str(),
        m_windowWidth,
        m_windowHeight,
        flags
    );

    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (m_glContext == nullptr) {
        std::cerr << "SDL_GL_CreateContext error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (version == 0) {
        std::cerr << "gladLoadGL error: failed to load OpenGL\n";
        return SDL_APP_FAILURE;
    }
    std::cout << "Using OpenGL " << glGetString(GL_VERSION) << "\n";

    if (!SDL_GL_SetSwapInterval(-1)) {
        std::cerr << "Adaptive vsync not supported!\n";
        SDL_GL_SetSwapInterval(1);
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_batcher->init();

    resize();

    return SDL_APP_CONTINUE;
}

void GlRenderer::beginFrame()
{
    glClearColor(
        m_clearColor.normalizedR(),
        m_clearColor.normalizedG(),
        m_clearColor.normalizedB(),
        m_clearColor.normalizedA()
    );
    glClear(GL_COLOR_BUFFER_BIT);

    m_batcher->beginBatch();
}

void GlRenderer::endFrame()
{
    m_batcher->flushBatch();
    SDL_GL_SwapWindow(window());
}

void GlRenderer::resize()
{
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    m_batcher->transform = Matrix3x3(
        {0, 0},
        0,
        {1.F / (m_windowWidth / 2.F), (1.F / (m_windowHeight / 2.F))}
    );
};
Texture* GlRenderer::createTexture()
{
    return new GlTexture();
}
