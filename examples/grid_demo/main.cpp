/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/app.h"
#include "fc/client/scene/sprite.h"
#include "fc/core/collision/grid.h"
#include "fc/core/collision/shape.h"

#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include <cstdlib>

inline constexpr size_t GRID_SIZE = 20;
inline constexpr size_t ITEM_SIZE = 32;
inline constexpr size_t WINDOW_SIZE = GRID_SIZE * ITEM_SIZE;

class GridSprite : public SpriteItem
{
public:
    GridSprite() : SpriteItem() { };
    Rect hitbox = Rect::fromDims(GRID_SIZE, GRID_SIZE);
};

class Benchmark : public GameApp
{
public:
    Benchmark() :
        GameApp({
            .width = WINDOW_SIZE,
            .height = WINDOW_SIZE,
            .resizable = false,
            .windowTitle = "Grid Test",
        })
    {
    }

    SDL_AppResult init(int /*argc*/, char** /*argv*/) override
    {
        renderer()->resources().loadTexture("bird", "assets/bird.png");

        for (size_t i = 0, y = 0, x = 0; i < m_sprites.size(); i++) {
            GridSprite& sprite = m_sprites[i];
            sprite.setTexture("bird");

            sprite.position.x = x + (ITEM_SIZE / 2.F);
            sprite.position.y = y + (ITEM_SIZE / 2.F);

            x += ITEM_SIZE;
            if (x >= WINDOW_SIZE) {
                y += ITEM_SIZE;
                x = 0;
            }

            sprite.width = ITEM_SIZE;
            sprite.height = ITEM_SIZE;
            sprite.hitbox.translate(sprite.position);

            m_grid.insertEntity(i, sprite.hitbox.min, sprite.hitbox.max);
            m_root.addChild(&sprite);
        }
        renderer()->setClearColor(0);

        return SDL_APP_CONTINUE;
    };

    SDL_AppResult update(float /*dt*/) override
    {
        Vec2F center = {WINDOW_SIZE / 2.F, WINDOW_SIZE / 2.F};

        m_root.position = -center;

        for (auto& sprite : m_sprites) {
            sprite.tint = 0xffffff;
        }

        const std::vector<uint16_t>* query;
        Vec2 mousePos = {m_inputManager.getMousePos().x, m_renderer->windowHeight() - m_inputManager.getMousePos().y};

        if (m_queryLine) {
            query = &m_grid.queryLine(m_lineStart, mousePos);
        } else {
            query = &m_grid.queryAABB(Vec2F::min(m_lineStart, mousePos), Vec2F::max(m_lineStart, mousePos));
        }

        for (auto id : *query) {
            m_sprites[id].tint = 0xff0000;
        }
        if (m_inputManager.isMouseBtnDown(1)) {
            m_lineStart = {m_inputManager.getMousePos().x, m_renderer->windowHeight() - m_inputManager.getMousePos().y};
        }

        if (m_inputManager.isKeyDown("K") && m_queryLine) {
            m_queryLine = false;
        }
        if (m_inputManager.isKeyDown("L") && !m_queryLine) {
            m_queryLine = true;
        }

        m_root.renderChildren(m_root.getMatrix(), renderer());
        return SDL_APP_CONTINUE;
    };

private:
    Container m_root;

    bool m_queryLine = true;

    Vec2F m_lineStart{WINDOW_SIZE / 2.F, WINDOW_SIZE / 2.F};

    Grid<uint16_t, uint16_t> m_grid{WINDOW_SIZE, ITEM_SIZE, (1 << 16) - 1};

    std::array<GridSprite, (GRID_SIZE * GRID_SIZE)> m_sprites;
};

INIT_APP(Benchmark)
