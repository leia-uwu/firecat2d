/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/inputManager.h"

bool InputManager::isKeyDown(const std::string& key)
{
    return m_keys[key];
}

bool InputManager::isMouseBtnDown(uint8_t btn)
{
    return m_mouseBtnsDown[btn];
};

Vec2F InputManager::getMousePos() const
{
    return m_mousePos;
}

Vec2F InputManager::getWheelDelta() const
{
    return m_wheelDelta;
}

void InputManager::processSDLEvent(SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_KEY_DOWN:
        m_keys.insert_or_assign(SDL_GetKeyName(event->key.key), true);
        break;
    case SDL_EVENT_KEY_UP:
        m_keys.insert_or_assign(SDL_GetKeyName(event->key.key), false);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        m_mousePos.set(event->button.x, event->button.y);
        m_mouseBtnsDown[event->button.button] = true;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        m_mousePos.set(event->button.x, event->button.y);
        m_mouseBtnsDown[event->button.button] = false;
        break;
    case SDL_EVENT_MOUSE_WHEEL:
        m_wheelDelta.set(event->wheel.x, event->wheel.y);
        break;
    case SDL_EVENT_MOUSE_MOTION:
        m_mousePos.set(event->motion.x, event->motion.y);
        break;
    }
};

void InputManager::flush()
{
    m_wheelDelta.set(0, 0);
}
