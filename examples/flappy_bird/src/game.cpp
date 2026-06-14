/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

#include <cstdlib>

#include "fc/client/resources.h"
#include "fc/core/collision/collision.h"
#include "fc/core/collision/shape.h"

#include "game.h"

#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkSurface.h>

SDL_AppResult Game::init(int /*argc*/, char** /*argv*/)
{
    SDL_SetWindowMinimumSize(renderer().window(), GAME_WIDTH, GAME_HEIGHT);
    renderer().setClearColor(0x141414);

    Resources::get().loadTexture("bird", "assets/bird.png");
    Resources::get().loadTexture("pipe", "assets/pipe.png");

    m_root.addChild(&m_scoreText);

    m_scoreText.setZIndex(99);
    m_scoreText.font().setTypeface(Resources::get().getFont("Inter"));
    m_scoreText.font().setSize(25);
    m_scoreText.fillPaint().setColor(0xffff0000);
    m_scoreText.strokePaint().setStrokeWidth(4);
    m_scoreText.strokePaint().setColor(0xffffffff);
    m_scoreText.setText("Score - 0");
    m_scoreText.position = {10, 20};

    return SDL_APP_CONTINUE;
}

SDL_AppResult Game::update(float dt)
{
    if (renderer().focused()) {
        //
        // update loop, only run when focused
        //

        if (inputManager().isKeyDown("W") || inputManager().isMouseBtnDown(1)) {
            m_player.vel.y = -500;
        }

        m_player.update(dt);

        m_lastPipeDistance += PIPE_SPEED * dt;

        if (m_lastPipeDistance > PIPE_DISTANCE) {
            m_lastPipeDistance = 0;

            addPipe();
        }

        m_player.sprite.tint = 0xaa55ff;

        for (auto& pipe : m_pipes) {
            pipe->update(dt);

            Collision::Response res;
            if (m_player.hitbox.getCollision(pipe->hitbox, &res)) {
                // m_player.hitbox.translate(res.normal * -res.depth);
                m_player.sprite.tint = 0xff0000;
            }
        }

        m_scoreTicker += dt;
        if (m_scoreTicker >= 1) {
            m_scoreTicker = 0;
            m_score++;
            m_scoreText.setText(std::format("Score - {}", m_score));
        }
    }
    //
    // RENDER
    //

    for (auto& pipe : m_pipes) {
        pipe->render();
    }

    m_player.render();

    // m_renderer.drawDebugShape(m_player.hitbox);
    // for (auto& pipe : m_pipes) {
    //     m_renderer.drawDebugShape(pipe->hitbox);
    // }
    // m_renderer.drawDebugShape(GAME_FLOOR);

    return SDL_APP_CONTINUE;
};

void Game::addPipe()
{
    const int pad = PIPE_GAP / 2;
    const int gapYMax = GAME_HEIGHT - PIPE_GAP - pad;

    const float randomT = std::rand() / (float)RAND_MAX;

    const float gapYStart = (randomT * (gapYMax - pad)) + pad;
    const float gapYEnd = gapYStart + PIPE_GAP;

    auto findPipe = [this](Vec2F pos, float width, float height) {
        for (auto& pipe : m_pipes) {
            if (pipe->hitbox.max.x < 0) {

                Rect r = Rect::fromDims(width, height, pos);
                pipe->hitbox.min = r.min;
                pipe->hitbox.max = r.max;

                return;
            }
        }

        m_pipes.emplace_back(new Pipe(pos, width, height, m_root));
    };

    findPipe(
        {GAME_WIDTH + (PIPE_WIDTH / 2), gapYStart / 2},
        PIPE_WIDTH,
        gapYStart
    );

    findPipe(
        {GAME_WIDTH + (PIPE_WIDTH / 2), gapYEnd + ((GAME_HEIGHT - gapYEnd) / 2)},
        PIPE_WIDTH,
        GAME_HEIGHT - gapYEnd
    );
}

Player::Player(Container& root) :
    hitbox(Rect::fromDims(PLAYER_SIZE, PLAYER_SIZE, {PLAYER_SIZE, PLAYER_SIZE}).getPoints())
{
    sprite.width = PLAYER_SIZE;
    sprite.height = PLAYER_SIZE;
    sprite.setTexture("bird");
    sprite.setZIndex(1);
    root.addChild(&sprite);
}

void Player::update(float dt)
{
    vel.y += GRAVITY * dt;

    Vec2F step = vel * dt;
    hitbox.translate(step);

    Collision::Response res;
    if (hitbox.getCollision(GAME_FLOOR, &res)) {
        hitbox.translate(res.normal * -res.depth);
        vel.y = 0;
    }

    hitbox.rotate(dt);
    sprite.rotation += dt;
}

void Player::render()
{
    sprite.position = hitbox.center();
}

Pipe::Pipe(
    Vec2F pos,
    float width,
    float height,
    Container& root
) :
    hitbox(Rect::fromDims(width, height, pos))
{
    sprite.setTexture("pipe");
    sprite.tint = 0x00ff00;
    root.addChild(&sprite);
}

void Pipe::update(float dt)
{
    Vec2F step = vel * dt;
    hitbox.translate(step);
}

void Pipe::render()
{
    sprite.position = hitbox.center();
    sprite.width = hitbox.width();
    sprite.height = hitbox.height();
}
