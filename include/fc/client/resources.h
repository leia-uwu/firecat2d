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

#include <include/core/SkFontMgr.h>
#include <include/core/SkTypeface.h>

#include "fc/client/render/texture.h"
#include "fc/client/render/typeFace.h"

class Resources
{
public:
    void loadTexture(const char* id, const char* path);

    [[nodiscard]] Texture* getTexture(const std::string& id);

    void loadTypeFace(const char* id, const char* path);

    [[nodiscard]] TypeFace* getTypeFace(const std::string& id);

    static Resources& get()
    {
        static Resources r{};
        return r;
    };

    sk_sp<SkFontMgr> fontManager()
    {
        return m_fontManager;
    }

private:
    Resources();

    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;

    sk_sp<SkFontMgr> m_fontManager;
    std::unordered_map<std::string, std::unique_ptr<TypeFace>> m_typeFaces;
};
