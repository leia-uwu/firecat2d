/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include <SDL3/SDL_init.h>
#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

#include "fc/client/app.h"
#include "fc/client/resources.h"
#include "fc/client/scene/container.h"
#include "fc/client/scene/sprite.h"
#include "fc/client/scene/text.h"
#include "fc/core/collision/shape.h"
#include "fc/core/math/vec2.h"

#include <cstdlib>
#include <memory>
#include <vector>

static const int GAME_WIDTH = 1024;
static const int GAME_HEIGHT = 512;
static const float PLAYER_SIZE = 64;

static const float GRAVITY = 1000;
static const float JUMP_SPEED = 450;
static const float PIPE_SPEED = 300;
static const float PIPE_WIDTH = 100;
static const float PIPE_GAP = 150;
static const float PIPE_DISTANCE = 400;

static const Rect GAME_FLOOR = Rect::fromDims(GAME_WIDTH, 20, {GAME_WIDTH / 2.F, GAME_HEIGHT + 10});
static const Rect GAME_CEILING = Rect::fromDims(GAME_WIDTH, 20, {GAME_WIDTH / 2.F, -10});

static const Polygon DEFAULT_PLAYER_HITBOX = Polygon{
    Rect::fromDims(
        PLAYER_SIZE, PLAYER_SIZE,
        {128, 256}
    )
        .getPoints()
};

static bool DEBUG_HITBOXES = false;

class Player
{
public:
    SpriteItem sprite;

    Vec2F vel;

    Polygon hitbox;

    Player(Container& root) :
        hitbox(DEFAULT_PLAYER_HITBOX)
    {
        sprite.width = PLAYER_SIZE;
        sprite.height = PLAYER_SIZE;
        sprite.setTexture("bird");
        sprite.setZIndex(1);
        sprite.tint = 0xaa55ff;
        root.addChild(&sprite);
    };

    void update(float dt)
    {
        vel.y += GRAVITY * dt;

        Vec2F step = vel * dt;
        hitbox.translate(step);

        Collision::Response res;
        if (hitbox.getCollision(GAME_FLOOR, &res)) {
            hitbox.translate(res.normal * -res.depth);
            vel.y = 0;
        }
        if (hitbox.getCollision(GAME_CEILING, &res)) {
            hitbox.translate(res.normal * -res.depth);
            vel.y = 0;
        }

        hitbox.rotate(dt);
        sprite.rotation += dt;
        sprite.position = hitbox.center();
    };

    void render()
    {
        sprite.position = hitbox.center();
    }
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
    ) :
        hitbox(Rect::fromDims(width, height, pos))
    {
        sprite.setTexture("pipe");
        sprite.tint = 0x00ff00;
        root.addChild(&sprite);
    }

    void update(float dt)
    {
        Vec2F step = vel * dt;
        hitbox.translate(step);
    };

    void render()
    {
        sprite.position = hitbox.center();
        sprite.width = hitbox.width();
        sprite.height = hitbox.height();
    };
};

class Game : public GameApp
{
public:
    Game() :
        GameApp({
            .width = GAME_WIDTH,
            .height = GAME_HEIGHT,
            .resizable = false,
            .windowTitle = "Flappy Bird!",
        })
    {
    }

    SDL_AppResult init(int /*argc*/, char** /*argv*/) override
    {
        renderer().setClearColor(0x141414);

        Resources::get().loadTexture("bird", "assets/bird.png");
        Resources::get().loadTexture("pipe", "assets/pipe.png");

        m_root.addChild(&m_scoreText);
        m_root.addChild(&m_titleText);

        Resources::get().loadTypeFace("Roboto", "fonts/Roboto-Variable.ttf");

        m_titleText.visible = true;
        m_titleText.setZIndex(100);
        m_titleText.setTypeFace(Resources::get().getTypeFace("Roboto"));
        m_titleText.font().setSize(45);
        m_titleText.fillPaint().setColor(0xffff0000);
        m_titleText.strokePaint().setStrokeWidth(4);
        m_titleText.strokePaint().setColor(0xffffffff);
        m_titleText.setText("Flappy Bird!");

        m_scoreText.visible = false;
        m_scoreText.setZIndex(99);
        m_scoreText.setTypeFace(Resources::get().getTypeFace("Roboto"));
        m_scoreText.font().setSize(25);
        m_scoreText.fillPaint().setColor(0xffff0000);
        m_scoreText.strokePaint().setStrokeWidth(4);
        m_scoreText.strokePaint().setColor(0xffffffff);
        m_scoreText.position = {20, 30};
        updateScoreText();

        return SDL_APP_CONTINUE;
    };

    SDL_AppResult update(float dt) override
    {
        if (inputManager().isKeyDown("W") || inputManager().isMouseBtnDown(1)) {
            if (m_state == State::TITLE_SCREEN
                // when the player dies only allow a restart after half a second
                || (m_state == State::DEAD && m_deadTicker > 0.5F)) {
                startPlaying();
            } else if (m_state == State::PAUSED) {
                unpause();
            }
        }
        if (!m_renderer.focused() && m_state == State::PLAYING) {
            pause();
        }

        if (m_state == State::PLAYING) {
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
                    m_player.sprite.tint = 0xff0000;
                    die();
                    break;
                }
            }

            m_scoreTicker += dt;
            if (m_scoreTicker >= 1) {
                m_scoreTicker = 0;
                m_score++;
                updateScoreText();
            }
        } else {
            if (m_state == State::DEAD) {
                m_deadTicker += dt;
                m_titleText.fillPaint().setAlphaf(m_deadTicker * 2.F);
                m_titleText.strokePaint().setAlphaf(m_deadTicker * 2.F);
            }
            positionTitle();
        }

        //
        // RENDER
        //

        for (auto& pipe : m_pipes) {
            pipe->render();
        }

        m_player.render();

        if (DEBUG_HITBOXES) {
            m_renderer.drawDebugShape(m_player.hitbox);
            for (auto& pipe : m_pipes) {
                m_renderer.drawDebugShape(pipe->hitbox);
            }
            m_renderer.drawDebugShape(GAME_FLOOR);
            m_renderer.drawDebugShape(GAME_CEILING);
        }

        return SDL_APP_CONTINUE;
    };

private:
    Player m_player{m_root};
    std::vector<std::unique_ptr<Pipe>> m_pipes;
    float m_lastPipeDistance = 0;

    float m_scoreTicker = 0;
    int m_score = 0;

    TextItem m_titleText;
    TextItem m_scoreText;

    enum class State : uint8_t {
        TITLE_SCREEN,
        PLAYING,
        PAUSED,
        DEAD
    };

    State m_state = State::TITLE_SCREEN;

    float m_deadTicker = 0;

    void positionTitle()
    {
        m_titleText.position.y = m_renderer.windowHeight() / 3.F;

        m_titleText.position.x = (m_renderer.windowWidth() / 2.F) - m_titleText.getWidth() / 2.F;
    }

    void updateScoreText()
    {
        m_scoreText.setText(std::format("Score: {:0>4}", m_score));
    }

    void startPlaying()
    {
        assert(m_state == State::TITLE_SCREEN || m_state == State::DEAD);

        m_pipes.clear();
        m_score = 0;
        m_scoreTicker = 0;
        updateScoreText();

        m_state = State::PLAYING;

        m_scoreText.visible = true;
        m_titleText.visible = false;

        m_player.hitbox = DEFAULT_PLAYER_HITBOX;
        m_player.sprite.rotation = 0;
        m_player.vel = {};
    }

    void pause()
    {
        assert(m_state == State::PLAYING);

        m_state = State::PAUSED;
        m_titleText.visible = true;
        m_titleText.setText("Paused!");
    }

    void unpause()
    {
        assert(m_state == State::PAUSED);
        m_state = State::PLAYING;
        m_titleText.visible = false;
    }

    void die()
    {
        assert(m_state == State::PLAYING);

        m_state = State::DEAD;
        m_deadTicker = 0;
        m_titleText.visible = true;
        m_titleText.setText("You died lol!");

        m_titleText.fillPaint().setAlphaf(0);
        m_titleText.strokePaint().setAlphaf(0);
    }

    void addPipe()
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
    };
};

INIT_APP(Game)
