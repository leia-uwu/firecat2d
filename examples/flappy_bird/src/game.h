/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/app.h"
#include "fc/client/scene/container.h"
#include "fc/client/scene/sprite.h"

#include "fc/core/collision/shape.h"
#include "fc/core/math/vec2.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <memory>

static const float GAME_WIDTH = 1024;
static const float GAME_HEIGHT = 512;
static const float PLAYER_SIZE = 64;

static const float GRAVITY = 1000;
static const float JUMP_SPEED = 450;
static const float PIPE_SPEED = 300;
static const float PIPE_WIDTH = 100;
static const float PIPE_GAP = 150;
static const float PIPE_DISTANCE = 400;

static const Rect GAME_FLOOR = Rect::fromDims(GAME_WIDTH, 20, {GAME_WIDTH / 2, GAME_HEIGHT + 10});

class Player
{
public:
    SpriteItem sprite;

    Vec2F vel;

    Polygon hitbox;

    Player(Container& root);

    void update(float dt);

    void render();
};

class Pipe
{
public:
    SpriteItem sprite;

    Vec2F vel{-PIPE_SPEED, 0};

    Rect hitbox;

    Pipe(
        Vec2F pos,
        float width,
        float height,
        Container& root
    );

    void update(float dt);

    void render();
};

class Game : public GameApp
{
public:
    Game() :
        GameApp({
            .width = 800,
            .height = 450,
            .resizable = false,
            .windowTitle = "Flappy Bird!",
        })
    {
    }

    SDL_AppResult init(int argc, char** argv) override;

    SDL_AppResult update(float dt) override;

private:
    Player m_player{m_root};
    std::vector<std::unique_ptr<Pipe>> m_pipes;
    float m_lastPipeDistance = 0;

    void addPipe();
};
