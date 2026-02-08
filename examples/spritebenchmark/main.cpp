/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "fc/client/app.h"
#include "fc/client/scene/sprite.h"

#include <algorithm>
#include <cstdlib>

class BenchSprite : public SpriteItem
{
public:
    BenchSprite() : SpriteItem() { };
    Vec2F velocity;
    float rotationDir;
};

class Benchmark : public GameApp
{
public:
    Benchmark() :
        GameApp({
            .width = 800,
            .height = 800,
            .resizable = false,
            .windowTitle = "OBS bitrate killer",
        })
    {
        ticker().toggleProfiling(true);
    }

    SDL_AppResult init(int /*argc*/, char** /*argv*/) override
    {
        renderer()->resources().loadTexture("bird", "assets/bird.png");

        for (auto& sprite : m_sprites) {
            sprite.setTexture("bird");
            sprite.width = 64;
            sprite.height = 64;
            sprite.tint = std::rand();
            sprite.position.x = renderer()->windowWidth() / 2.F;
            sprite.position.y = renderer()->windowHeight() / 2.F;
            sprite.velocity.x = ((std::rand() / (float)RAND_MAX) * 1000) - 500;
            sprite.velocity.y = ((std::rand() / (float)RAND_MAX) * 1000) - 500;
            sprite.rotationDir = ((std::rand() / (float)RAND_MAX) * 20) - 10;
            m_root.addChild(&sprite);
        }
        renderer()->setClearColor(0);

        return SDL_APP_CONTINUE;
    };

    SDL_AppResult update(float dt) override
    {
        m_root.position.x = -(renderer()->windowWidth() / 2.F);
        m_root.position.y = -(renderer()->windowHeight() / 2.F);
        for (auto& sprite : m_sprites) {
            sprite.position += sprite.velocity * dt;

            int width = renderer()->windowWidth();
            int height = renderer()->windowHeight();

            if (sprite.position.x < 0 || sprite.position.x > width) {
                sprite.velocity.x = -sprite.velocity.x;
            }
            if (sprite.position.y < 0 || sprite.position.y > height) {
                sprite.velocity.y = -sprite.velocity.y;
            }

            sprite.rotation += sprite.rotationDir * dt;

            sprite.position.x = std::clamp(sprite.position.x, 0.F, (float)width);
            sprite.position.y = std::clamp(sprite.position.y, 0.F, (float)height);
        }

        m_root.renderChildren(m_root.getMatrix(), renderer());
        return SDL_APP_CONTINUE;
    };

private:
    Container m_root;
    std::array<BenchSprite, 9999> m_sprites;
};

INIT_APP(Benchmark)
