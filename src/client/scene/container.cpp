/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/scene/container.h"
#include <algorithm>

Container& Container::addChild(Container* child)
{
    assert(child != nullptr);
    assert(child != this); // why are you trying to add a node to itself lol

    if (child->m_parent == this) {
        return *this;
    }

    if (child->m_parent != nullptr) {
        child->m_parent->removeChild(child);
    }

    m_children.push_back(child);
    child->m_parent = this;

    m_sortDirty = true;
    return *this;
}

Container& Container::addChild(std::initializer_list<Container*> children)
{
    for (auto* child : children) {
        addChild(child);
    }
    return *this;
}

Container& Container::removeChild(Container* child)
{
    assert(child->m_parent == this);

    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        if (*it == child) {
            m_children.erase(it);
            break;
        }
    }
    child->m_parent = nullptr;

    return *this;
}

Container& Container::clear()
{
    for (auto* child : m_children) {
        child->m_parent = nullptr;
    }
    m_children.clear();

    return *this;
}

Container& Container::setZIndex(int32_t idx)
{
    if (m_zIndex == idx)
        return *this;

    m_zIndex = idx;
    if (m_parent != nullptr) {
        m_parent->m_sortDirty = true;
    }
    return *this;
}

void Container::sortChildren()
{
    std::ranges::sort(m_children, {}, &Container::m_zIndex);
}

void Container::renderChildren(const Matrix3x3& transform, Renderer* renderer)
{
    if (m_sortDirty) {
        m_sortDirty = false;
        sortChildren();
    }
    for (Container* child : m_children) {
        auto childT = Matrix3x3::mul(transform, child->getMatrix());
        child->renderChildren(childT, renderer);
        child->render(childT, renderer);
    }
}

void Container::destroyChildren()
{
    for (auto* child : m_children) {
        child->destroyChildren();
        delete child;
    }
    m_children.clear();
}

Container::~Container()
{
    if (m_parent != nullptr) {
        m_parent->removeChild(this);
    }

    for (auto* child : m_children) {
        child->m_parent = nullptr;
    }
}
