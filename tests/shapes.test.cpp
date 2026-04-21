/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/core/collision/shape.h"

#include <doctest/doctest.h>
#include <memory>

TEST_CASE("Shapes")
{
    SUBCASE("Circle constructors")
    {
        Circle circleA{{}, 10};
        CHECK(circleA.pos == Vec2F{0, 0});
        CHECK(circleA.rad == 10.F);

        Circle circleB{{30, 50}, 40};
        CHECK(circleB.pos == Vec2F{30, 50});
        CHECK(circleB.rad == 40.F);

        circleA = circleB;
        CHECK(circleA.pos == Vec2F{30, 50});
        CHECK(circleA.rad == 40.F);

        std::unique_ptr<Circle> circleC = std::make_unique<Circle>(Circle{{10, 20}, 10});
        CHECK(circleC->pos == Vec2F{10, 20});
        CHECK(circleC->rad == 10.F);
    }

    SUBCASE("Rect constructors")
    {
        Rect rectA{{}, {10, 10}};
        CHECK(rectA.min == Vec2F{0, 0});
        CHECK(rectA.max == Vec2F{10, 10});

        Rect rectB{{30, 50}, {40, 60}};
        CHECK(rectB.min == Vec2F{30, 50});
        CHECK(rectB.max == Vec2F{40, 60});

        rectA = rectB;
        CHECK(rectA.min == Vec2F{30, 50});
        CHECK(rectA.max == Vec2F{40, 60});

        std::unique_ptr<Rect> rectC = std::make_unique<Rect>(Rect{{10, 20}, {20, 30}});
        CHECK(rectC->min == Vec2F{10, 20});
        CHECK(rectC->max == Vec2F{20, 30});
    }

    SUBCASE("Rect points should be valid polygon")
    {
        Rect rect{{}, {10, 10}};

        auto points = rect.getPoints();
        CHECK(Polygon::isCounterClockwise(points[0], points[1], points[2]));
        CHECK(Polygon::isConvex(points));
    }

    SUBCASE("Polygon constructors")
    {
        Polygon polyA{{{10, 10}, {10, 20}, {20, 20}, {20, 10}}};
        CHECK(polyA.points[0] == Vec2F{10, 10});
        CHECK(polyA.points[1] == Vec2F{10, 20});
        CHECK(polyA.points[2] == Vec2F{20, 20});
        CHECK(polyA.points[3] == Vec2F{20, 10});

        CHECK(polyA.normals()[0] == Vec2F::up());
        CHECK(polyA.normals()[1] == Vec2F::left());
        CHECK(polyA.normals()[2] == Vec2F::down());
        CHECK(polyA.normals()[3] == Vec2F::right());

        CHECK(polyA.center() == Vec2F{15, 15});

        Polygon polyB = Polygon::fromSides(7, {}, 10);
        CHECK(polyB.center().equals(Vec2F{0, 0}, 0.00001));
    }
}
