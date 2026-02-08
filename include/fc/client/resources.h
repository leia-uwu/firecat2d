/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>

#include "fc/client/render/texture.h"

class Renderer;

class ResourceManager
{
public:
    void loadTexture(const char* id, const char* path);

    [[nodiscard]] Texture* getTexture(const std::string& id);

    void setRenderer(Renderer* renderer);

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;

    Renderer* m_renderer = nullptr;
};
