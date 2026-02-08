/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/renderer.h"

#include <SDL3/SDL_video.h>

Renderer::Renderer(const InitFlags& flags) :
    m_windowWidth(flags.width),
    m_windowHeight(flags.height),
    m_resizable(flags.resizable),
    m_windowTitle(flags.windowTitle)
{
    resources().setRenderer(this);
};

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
