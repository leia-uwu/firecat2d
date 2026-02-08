/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/resources.h"
#include "fc/client/render/renderer.h"
#include "fc/core/buffer.h"

#include <SDL3/SDL_filesystem.h>
#include <cstdint>
#include <functional>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#include <filesystem>
#include <fstream>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

static void loadFile(const std::string& path, const std::function<void(bool success, Uint8Buffer buffer)>& cb)
{
#ifndef __EMSCRIPTEN__
    // oh my god why is the c++ api for this so ugly
    // i'm almost tempted to use the C api instead
    std::ifstream file(path, std::ios::binary | std::ios::in);

    if (file.is_open()) {
        std::cout << "Loading file " << path << "\n";
        size_t size = std::filesystem::file_size(path);
        if (size == 0) {
            cb(false, {nullptr, 0});
            std::cerr << "File " << path << " is empty\n";
        }
        auto* data = new uint8_t[size];
        file.seekg(0, std::ios::beg);
        file.read((char*)data, size);
        file.close();

        cb(true, {data, size});
        delete[] data;
    } else {
        std::cerr << "Failed to load file " << path << "\n";
        cb(false, {nullptr, 0});
    }
#else
    struct LoadCTX
    {
        std::string path;
        std::function<void(bool success, Uint8Buffer buffer)> cb;
    };
    auto* ctx = new LoadCTX{
        .path = path,
        .cb = cb
    };

    std::cout << "Loading " << path << "\n";

    emscripten_async_wget_data(
        path.c_str(),
        ctx,
        [](void* userData, void* buffer, int size) {
            auto* ctx = (LoadCTX*)userData;

            ctx->cb(true, {(uint8_t*)buffer, (size_t)size});
            delete ctx;
        },
        [](void* data) {
            auto* ctx = (LoadCTX*)data;
            std::cerr << "Failed to load file " << ctx->path << "\n";
            ctx->cb(false, {nullptr, 0});
            delete ctx;
        }
    );
#endif
}

void ResourceManager::loadTexture(const char* id, const char* path)
{
    assert(m_renderer);

    m_textures[id] = std::unique_ptr<Texture>(m_renderer->createTexture());
    Texture* texture = m_textures[id].get();
#ifndef __EMSCRIPTEN__
    std::string filePath = SDL_GetBasePath();
#else
    std::string filePath = "./";
#endif
    filePath += path;

    loadFile(filePath, [texture, this](bool success, Uint8Buffer buffer) {
        if (!success)
            return;

        int width = 0;
        int height = 0;
        int nrChannels = 0;
        // TODO: find a way to decode the image using js built in stuff instead of stb
        uint8_t* data = stbi_load_from_memory(buffer.data(), buffer.size(), &width, &height, &nrChannels, 4);

        if (data == nullptr) {
            std::cout << "Failed to load texture " << stbi_failure_reason() << "\n";
        } else {
            texture->generate(width, height, data);
            stbi_image_free(data);
        }
    });
}

Texture* ResourceManager::getTexture(const std::string& id)
{
    assert(m_textures.contains(id));

    return m_textures[id].get();
}

void ResourceManager::setRenderer(Renderer* renderer)
{
    m_renderer = renderer;
}
