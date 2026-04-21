/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/core/collision/shape.h"

#include <cfloat>

using CollisionFn = bool (*)(const Shape&, const Shape&, Collision::Response*);

struct CollisionFnData
{
    CollisionFn fn;
    bool reverse;
};

class CollisionFns
{
private:
    std::array<std::array<CollisionFnData, Shape::COUNT>, Shape::COUNT> m_fns;

    void registerFn(Shape::Type typeA, Shape::Type typeB, const CollisionFn& fn)
    {
        m_fns[typeA][typeB] = {
            .fn = fn,
            .reverse = false
        };

        if (typeA != typeB) {
            m_fns[typeB][typeA] = {
                .fn = fn,
                .reverse = true
            };
        }
    };

public:
    CollisionFns()
    {
        registerFn(Shape::CIRCLE, Shape::CIRCLE, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
            assert(shapeA.type == Shape::CIRCLE);
            assert(shapeB.type == Shape::CIRCLE);

            const auto& a = static_cast<const Circle&>(shapeA);
            const auto& b = static_cast<const Circle&>(shapeB);
            return CircleCircle(a.pos, a.rad, b.pos, b.rad, res);
        });
        registerFn(Shape::CIRCLE, Shape::RECT, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
            assert(shapeA.type == Shape::CIRCLE);
            assert(shapeB.type == Shape::RECT);

            const auto& a = static_cast<const Circle&>(shapeA);
            const auto& b = static_cast<const Rect&>(shapeB);
            return CircleRect(a.pos, a.rad, b.min, b.max, res);
        });
        registerFn(Shape::CIRCLE, Shape::POLYGON, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
            assert(shapeA.type == Shape::CIRCLE);
            assert(shapeB.type == Shape::POLYGON);

            const auto& a = static_cast<const Circle&>(shapeA);
            const auto& b = static_cast<const Polygon&>(shapeB);
            return CirclePolygon(a.pos, a.rad, b.points, b.normals(), b.center(), res);
        });
        registerFn(Shape::RECT, Shape::RECT, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
            assert(shapeA.type == Shape::RECT);
            assert(shapeB.type == Shape::RECT);

            const auto& a = static_cast<const Rect&>(shapeA);
            const auto& b = static_cast<const Rect&>(shapeB);
            return RectRect(a.min, a.max, b.min, b.max, res);
        });
        registerFn(Shape::RECT, Shape::POLYGON, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
            assert(shapeA.type == Shape::RECT);
            assert(shapeB.type == Shape::POLYGON);

            const auto& a = static_cast<const Rect&>(shapeA);
            const auto& b = static_cast<const Polygon&>(shapeB);
            return RectPolygon(a.min, a.max, b.points, b.normals(), b.center(), res);
        });
        registerFn(Shape::POLYGON, Shape::POLYGON, [](const Shape& shapeA, const Shape& shapeB, auto* res) {
            assert(shapeA.type == Shape::POLYGON);
            assert(shapeB.type == Shape::POLYGON);

            const auto& a = static_cast<const Polygon&>(shapeA);
            const auto& b = static_cast<const Polygon&>(shapeB);
            return PolygonPolygon(a.points, a.normals(), a.center(), b.points, b.normals(), b.center(), res);
        });
    };

    bool check(const Shape& shapeA, const Shape& shapeB, Collision::Response* res) const
    {
        assert(shapeA.type < Shape::COUNT);
        assert(shapeB.type < Shape::COUNT);

        const auto& collisionFn = m_fns[shapeA.type][shapeB.type];

        if (collisionFn.reverse) {
            bool collided = collisionFn.fn(shapeB, shapeA, res);
            res->normal.invert();
            return collided;
        }

        return collisionFn.fn(shapeA, shapeB, res);
    };
};

bool Shape::getCollision(const Shape& other, Collision::Response* res) const
{
    static const CollisionFns fns;
    return fns.check(*this, other, res);
}

Circle::Circle(Vec2F pos, float rad) :
    Shape(CIRCLE),
    pos(pos),
    rad(rad)
{
    assert(rad >= 0);
}

Circle::Circle(const Circle& circ) :
    Circle(circ.pos, circ.rad)
{
}

Circle::Circle(Circle&& circ) noexcept :
    Shape(CIRCLE)
{
    swap(*this, circ);
}

Circle& Circle::operator=(Circle circ)
{
    swap(*this, circ);
    return *this;
}

void Circle::swap(Circle& lhs, Circle& rhs) noexcept
{
    using std::swap;

    swap(lhs.pos, rhs.pos);
    swap(lhs.rad, rhs.rad);
}

std::string Circle::toString() const
{
    return std::format("Circle (X: {0:.4f}, Y: {1:.4f}, Rad: {2:.4f})", pos.x, pos.y, rad);
}

bool Circle::pointInside(Vec2F point) const
{
    return Collision::PointCircle(point, pos, rad);
}

Vec2F Circle::center() const
{
    return pos;
}

Circle& Circle::translate(Vec2F posToAdd)
{
    pos += posToAdd;
    return *this;
}

Circle& Circle::scale(float scale)
{
    rad *= scale;
    return *this;
}

std::pair<Vec2F, Vec2F> Circle::getAABB() const
{
    return {
        {pos.x - rad, pos.y - rad},
        {pos.x + rad, pos.y + rad}
    };
};

Rect::Rect(Vec2F min, Vec2F max) :
    Shape(RECT),
    min(min),
    max(max)
{
    // min being bigger than max can cause issues with collision functions
    assert(min.x < max.x);
    assert(min.y < max.y);
}

Rect::Rect(const Rect& rect) :
    Rect(rect.min, rect.max)
{
}

Rect::Rect(Rect&& rect) noexcept :
    Shape(RECT)
{
    swap(*this, rect);
}

void Rect::swap(Rect& lhs, Rect& rhs) noexcept
{
    using std::swap;

    swap(lhs.min, rhs.min);
    swap(lhs.max, rhs.max);
}

Rect& Rect::operator=(Rect rect)
{
    swap(*this, rect);
    return *this;
}

Rect Rect::fromDims(float width, float height, Vec2F center)
{
    Vec2F size{width / 2, height / 2};

    return Rect{center - size, center + size};
}

Rect& Rect::scale(float scale)
{
    Vec2F center = this->center();

    min = (min - center) * scale + center;
    max = (max - center) * scale + center;

    return *this;
}

std::pair<Vec2F, Vec2F> Rect::getAABB() const
{
    return {min, max};
};

Rect& Rect::translate(Vec2F posToAdd)
{
    min += posToAdd;
    max += posToAdd;

    return *this;
}

bool Rect::pointInside(Vec2F point) const
{
    return Collision::PointRect(point, min, max);
}

std::string Rect::toString() const
{
    return std::format("Rect(Min ({}) Max ({}))", min.toString(), max.toString());
}

std::vector<Vec2F> Rect::getPoints() const
{
    return {
        min,
        {min.x, max.y},
        max,
        {max.x, min.y},
    };
}

Vec2F Rect::center() const
{
    return min + ((max - min) / 2);
}

Polygon::Polygon(std::vector<Vec2F> points) :
    Shape(POLYGON),
    points(std::move(points)),
    m_normals(this->points.size())
{
    assert(this->points.size() >= 3);
    assert(isCounterClockwise(this->points[0], this->points[1], this->points[2]));
    assert(isConvex(this->points));

    calculateNormals();
    calculateCenter();
}

Polygon::Polygon(const Polygon& poly) : Polygon(poly.points)
{
}

Polygon::Polygon(Polygon&& poly) noexcept : Shape(POLYGON)
{
    swap(*this, poly);
}

void Polygon::swap(Polygon& lhs, Polygon& rhs) noexcept
{
    using std::swap;

    swap(lhs.points, rhs.points);
    swap(lhs.m_normals, rhs.m_normals);
    swap(lhs.m_center, rhs.m_center);
}

Polygon& Polygon::operator=(Polygon poly)
{
    swap(*this, poly);
    return *this;
}

Polygon Polygon::fromSides(size_t sides, Vec2F center, float radius)
{
    std::vector<Vec2F> points;
    points.resize(sides);

    float step = (M_PI * 2) / sides;

    for (size_t i = 0; i < sides; i++) {
        float angle = M_PI_2 - (step * i);

        Vec2F offset = {
            std::cos(angle) * radius,
            std::sin(angle) * radius,
        };

        points[i] = center + offset;
    }

    return Polygon{points};
}

Polygon& Polygon::scale(float scale)
{
    for (auto& pt : points) {
        Vec2F toCenter = m_center - pt;
        float length = toCenter.length();
        Vec2F dir = toCenter.normalize(length);

        pt = m_center - (dir * (length * scale));
    }

    return *this;
}

Polygon& Polygon::translate(Vec2F posToAdd)
{
    for (auto& point : points) {
        point += posToAdd;
    }
    m_center += posToAdd;

    return *this;
}

std::pair<Vec2F, Vec2F> Polygon::getAABB() const
{
    Vec2F min{FLT_MAX, FLT_MAX};
    Vec2F max{-FLT_MAX, -FLT_MAX};
    for (const Vec2F& pt : points) {
        min = Vec2F::min(pt, min);
        max = Vec2F::max(pt, max);
    }
    return {min, max};
};

Polygon& Polygon::rotate(float rotation)
{
    for (auto& point : points) {
        float dist = point.distanceTo(m_center);

        Vec2F dir = (point - m_center).normalize().rotate(-rotation).normalize();

        point = m_center - (dir * dist);
    }

    calculateNormals();

    return *this;
};

bool Polygon::pointInside(Vec2F point) const
{
    return Collision::PointPolygon(point, points);
}

std::string Polygon::toString() const
{
    std::string out = "Polygon [";

    for (size_t i = 0, size = points.size(); i < size; i++) {
        out += std::format("({})", points[i].toString());
        if (i != size - 1) {
            out += ", ";
        }
    }
    out += "]";

    return out;
}

Vec2F Polygon::center() const
{
    return m_center;
}

void Polygon::calculateCenter()
{
    m_center = {0, 0};
    for (const auto& point : points) {
        m_center += point;
    }
    m_center /= points.size();
}

void Polygon::calculateNormals()
{
    size_t len = points.size();
    for (
        size_t i = 0, j = len - 1;
        i < len;
        j = i++) {
        Vec2F pointA = points[j];
        Vec2F pointB = points[i];
        Vec2F edge = pointB - pointA;

        m_normals[i] = edge.perp().normalize();
    }
}

[[nodiscard]] bool Polygon::isCounterClockwise(Vec2F a, Vec2F b, Vec2F c)
{
    float d1 = b.x * a.y + c.x * b.y + a.x * c.y;
    float d2 = a.x * b.y + b.x * c.y + c.x * a.y;
    // d1 > d2 | counter-clockwise
    // d1 = d2 | collinear
    // d1 < d2 | clockwise
    // note: Y dependent, need to invert this check if Y goes upwards
    return d1 > d2;
}

[[nodiscard]] bool Polygon::isConvex(const std::vector<Vec2F>& points)
{
    size_t len = points.size();

    int8_t lastCross = 0;
    for (
        int i = 0, j = len - 1, k = j - 1;
        i < len;
        k = j++, j = i++
    ) {
        Vec2F pointA = points[i];
        Vec2F pointB = points[j];
        Vec2F pointC = points[k];

        int8_t cross = (pointC - pointB).cross(pointB - pointA) > 0 ? 1 : -1;

        if (i != 0 && cross != lastCross) {
            return false;
        }
        lastCross = cross;
    }

    return true;
}
