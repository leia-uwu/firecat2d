/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/color.h"
#include "fc/client/render/batcher.h"
#include "fc/client/resources.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

/**
 * Class to help manage and SDL Window and OpenGL Renderer
 */
class Renderer
{
public:
    struct InitFlags
    {
        size_t width = 800;
        size_t height = 400;
        bool resizable = true;
        std::string windowTitle = "Game";
    };

    Renderer(const InitFlags& flags);
    Renderer(const Renderer&) = delete;

    virtual ~Renderer() = default;

    /**
     * Tries to initialize SDL, the window and rendering context
     * If it fails you should exit the program
     */
    virtual SDL_AppResult init() = 0;

    void processSDLEvent(SDL_Event* event);

    [[nodiscard]] SDL_Window* window() const;

    bool setWindowTitle(const std::string& title);

    [[nodiscard]] std::string windowTitle() const;

    [[nodiscard]] int windowWidth() const;
    [[nodiscard]] int windowHeight() const;

    bool setWindowSize(int width, int height);

    [[nodiscard]] bool resizable() const;
    void setResizable(bool resizable);
    [[nodiscard]] bool focused() const;

    ResourceManager& resources()
    {
        return m_resources;
    }

    virtual RenderBatcher* batcher() = 0;

    Color clearColor()
    {
        return m_clearColor;
    }

    void setClearColor(const Color& color)
    {
        m_clearColor = color;
    }

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual Texture* createTexture() = 0;

protected:
    SDL_Window* m_window = nullptr;

    std::string m_windowTitle;

    int m_windowWidth;
    int m_windowHeight;

    bool m_resizable = true;
    bool m_focused = true;

    virtual void resize() = 0;

    Color m_clearColor;

    ResourceManager m_resources;
};
