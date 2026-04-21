/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/renderer.h"
#include "fc/core/collision/shape.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_video.h>

#include <iostream>
#include <utility>

#include <include/core/SkCanvas.h>
#include <include/core/SkColorSpace.h>
#include <include/core/SkSurface.h>
#include <include/gpu/ganesh/GrBackendSurface.h>
#include <include/gpu/ganesh/GrDirectContext.h>
#include <include/gpu/ganesh/SkSurfaceGanesh.h>
#include <include/gpu/ganesh/gl/GrGLBackendSurface.h>
#include <include/gpu/ganesh/gl/GrGLDirectContext.h>
#include <src/gpu/ganesh/gl/GrGLUtil.h>

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

inline constexpr int SAMPLES = 4;
inline constexpr int STENCIL_SIZE = 8;

Renderer::Renderer(const InitFlags& flags) :
    m_windowWidth(flags.width),
    m_windowHeight(flags.height),
    m_resizable(flags.resizable),
    m_windowTitle(flags.windowTitle) { };

Renderer::~Renderer()
{
    SDL_DestroyWindow(m_window);
    SDL_GL_DestroyContext(m_glContext);
}

SDL_AppResult Renderer::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    };

#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, SAMPLES);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_SIZE);

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

    if (!SDL_GL_MakeCurrent(m_window, m_glContext)) {
        std::cerr << "SDL_GL_MakeCurrent error: " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, m_window, 0, resizeCanvas);
    resizeCanvas(0, nullptr, this);
#endif

    if (!SDL_GL_SetSwapInterval(-1)) {
        std::cerr << "Adaptive vsync not supported!\n";
        SDL_GL_SetSwapInterval(1);
    };

    m_interface = GrGLMakeNativeInterface();
    if (m_interface.get() == nullptr) {
        std::cerr << "GrGLMakeNativeInterface returned null\n";
        return SDL_APP_FAILURE;
    }

    auto version = GrGLGetDriverInfo(m_interface.get()).fVersion;

    std::cout << "Using OpenGL " << GR_GL_MAJOR_VER(version) << "." << GR_GL_MINOR_VER(version) << "\n";

    GrContextOptions opts;
    opts.fSuppressPrints = true;
    m_context = GrDirectContexts::MakeGL(m_interface, opts);
    if (m_context.get() == nullptr) {
        std::cerr << "GrDirectContexts::MakeGL returned null\n";
        return SDL_APP_FAILURE;
    }

    resize();

    return SDL_APP_CONTINUE;
}

void Renderer::processSDLEvent(SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED:
        m_windowWidth = event->window.data1;
        m_windowHeight = event->window.data2;

        resize();
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        m_focused = false;
        break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        m_focused = true;
        break;
    }
}

SDL_Window* Renderer::window() const
{
    return m_window;
}

bool Renderer::setWindowTitle(const std::string& title)
{
    m_windowTitle = title;
    return SDL_SetWindowTitle(m_window, title.c_str());
}

std::string Renderer::windowTitle() const
{
    return m_windowTitle;
}

int Renderer::windowWidth() const
{
    return m_windowWidth;
}

int Renderer::windowHeight() const
{
    return m_windowHeight;
}

bool Renderer::setWindowSize(int width, int height)
{
    return SDL_SetWindowSize(m_window, width, height);
}

[[nodiscard]] bool Renderer::resizable() const
{
    return m_resizable;
}

void Renderer::setResizable(bool resizable)
{
    m_resizable = true;
    SDL_SetWindowResizable(m_window, resizable);
}

[[nodiscard]] bool Renderer::focused() const
{
    return m_focused;
}

void Renderer::clear()
{
    SkCanvas* canvas = m_surface->getCanvas();
    canvas->clear(SkColorSetARGB(m_clearColor.a, m_clearColor.r, m_clearColor.g, m_clearColor.b));
}

void Renderer::endFrame()
{
    SkCanvas* canvas = m_surface->getCanvas();

    if (auto* dContext = GrAsDirectContext(canvas->recordingContext())) {
        dContext->flushAndSubmit();
    }
}

void Renderer::drawDebugShape(const Shape& shape)
{
    auto* canvas = m_surface->getCanvas();
    canvas->save();
    SkPaint paint;
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(0xFFFF0000);
    paint.setStrokeWidth(2);

    switch (shape.type) {
    case Shape::CIRCLE: {
        const auto& circle = static_cast<const Circle&>(shape);
        canvas->drawCircle(circle.pos.x, circle.pos.y, circle.rad, paint);
        Vec2F lineEnd = circle.pos + (Vec2F::right() * circle.rad);
        canvas->drawLine(circle.pos.x, circle.pos.y, lineEnd.x, lineEnd.y, paint);
        break;
    }
    case Shape::RECT: {
        const auto& rect = static_cast<const Rect&>(shape);
        canvas->drawRect(SkRect::MakeLTRB(rect.min.x, rect.min.y, rect.max.x, rect.max.y), paint);
        break;
    }
    case Shape::POLYGON: {
        const auto& poly = static_cast<const Polygon&>(shape);

        Vec2F center = poly.center();

        canvas->drawPoint(center.x, center.y, paint);
        for (size_t i = 0, size = poly.points.size(); i < size; i++) {
            const Vec2F& prev = i == 0 ? poly.points[size - 1] : poly.points[i - 1];
            const Vec2F& cur = poly.points[i];
            canvas->drawLine(prev.x, prev.y, cur.x, cur.y, paint);

            Vec2F mid = (prev + cur) / 2;

            const Vec2F& normal = poly.normals()[i];
            Vec2F p2 = mid + (normal * 20);
            canvas->drawLine(mid.x, mid.y, p2.x, p2.y, paint);
        }
        break;
    }
    case Shape::COUNT:
        std::unreachable();
        break;
    }
    canvas->restore();
}

void Renderer::resize()
{
    GrGLFramebufferInfo info;
    info.fFormat = GR_GL_RGBA8;

#ifndef __EMSCRIPTEN__
    GR_GL_CALL(m_interface.get(), Viewport(0, 0, m_windowWidth, m_windowHeight));
    // Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
    // render to it
    GrGLint buffer;
    GR_GL_GetIntegerv(m_interface.get(), GR_GL_FRAMEBUFFER_BINDING, &buffer);

    info.fFBOID = (GrGLuint)buffer;
#else
    info.fFBOID = 0;
#endif

    GrBackendRenderTarget target = GrBackendRenderTargets::MakeGL(
        m_windowWidth,
        m_windowHeight,
        SAMPLES,
        STENCIL_SIZE,
        info
    );

    SkSurfaceProps props;

    m_surface = SkSurfaces::WrapBackendRenderTarget(
        m_context.get(),
        target,
        kBottomLeft_GrSurfaceOrigin,
        kRGBA_8888_SkColorType,
        nullptr,
        &props
    );
};
