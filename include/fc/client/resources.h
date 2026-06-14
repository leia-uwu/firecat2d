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

class Resources
{
public:
    void loadTexture(const char* id, const char* path);

    [[nodiscard]] Texture* getTexture(const std::string& id);

    void loadFont(const char* id, const char* path);

    [[nodiscard]] sk_sp<SkTypeface> getFont(const std::string& id);

    static Resources& get()
    {
        static Resources r{};
        return r;
    };

private:
    Resources();

    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;

    sk_sp<SkFontMgr> m_fontManager;
    std::unordered_map<std::string, sk_sp<SkTypeface>> m_fonts;
};
