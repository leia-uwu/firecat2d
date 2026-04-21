/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/app.h"
#include "fc/core/collision/collision.h"
#include "fc/core/collision/shape.h"
#include "fc/core/math/vec2.h"
#include <include/core/SkPaint.h>
#include <memory>

#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

class Shapes : public GameApp
{
public:
    Shapes() :
        GameApp({
            .width = 600,
            .height = 600,
            .resizable = true,
            .windowTitle = "Shapes",
        })
    {
    }

    SDL_AppResult init(int /*argc*/, char** /*argv*/) override
    {
        renderer().setClearColor(0);

        m_shapes.push_back(
            std::make_unique<Rect>(Vec2F{10, 10}, Vec2F{50, 50})
        );
        m_shapes.push_back(
            std::make_unique<Rect>(Vec2F{50, 450}, Vec2F{100, 500})
        );
        m_shapes.push_back(
            std::make_unique<Rect>(Rect::fromDims(40, 100, Vec2F{500, 100}))
        );

        m_shapes.push_back(
            std::make_unique<Rect>(Rect::fromDims(200, 100, Vec2F{450, 400}))
        );

        m_shapes.push_back(
            std::make_unique<Circle>(Vec2F{500, 300}, 80.F)
        );

        m_shapes.push_back(
            std::make_unique<Circle>(Vec2F{300, 500}, 20.F)
        );

        m_shapes.push_back(
            std::make_unique<Polygon>(Polygon::fromSides(3, Vec2F{200.F, 200.F}, 60).rotate(0.8))
        );

        m_shapes.push_back(
            std::make_unique<Polygon>(Polygon::fromSides(6, Vec2F{100.F, 300.F}, 40))
        );
        return SDL_APP_CONTINUE;
    };

    SDL_AppResult update(float /*dt*/) override
    {

        Vec2 mousePos = {m_inputManager.getMousePos().x, m_inputManager.getMousePos().y};

        if (m_inputManager.isMouseBtnDown(1)) {
            for (auto& shape : m_shapes) {
                if (shape->pointInside(mousePos)) {
                    Vec2F delta = mousePos - shape->center();
                    shape->translate(delta);
                    break;
                }
            }
        }

        for (auto& shapeA : m_shapes) {
            for (auto& shapeB : m_shapes) {
                if (shapeA.get() == shapeB.get())
                    continue;

                Collision::Response res;
                if (shapeA->getCollision(*shapeB, &res)) {
                    shapeB->translate(res.normal * res.depth);
                }
            }
        }
        for (auto& shape : m_shapes) {
            renderer().drawDebugShape(*shape);
        }

        return SDL_APP_CONTINUE;
    };

private:
    std::vector<std::unique_ptr<Shape>> m_shapes;
};

INIT_APP(Shapes)
