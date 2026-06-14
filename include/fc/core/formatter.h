/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

// reference: https://en.wikipedia.org/wiki/ANSI_escape_code
// inspired by https://github.com/chalk/chalk

#include <cstdint>
#include <memory>
#include <string>

#define MAKE_STYLE(name, open, close)                                                             \
private:                                                                                          \
    /* cache instances */                                                                         \
    mutable std::unique_ptr<Formatter> m_cached_##name{nullptr};                                  \
                                                                                                  \
public:                                                                                           \
    [[nodiscard]] inline Formatter& name() const                                                  \
    {                                                                                             \
        if (this->m_cached_##name == nullptr) {                                                   \
            this->m_cached_##name = std::unique_ptr<Formatter>(new Formatter(open, close, this)); \
        }                                                                                         \
        return *this->m_cached_##name;                                                            \
    }                                                                                             \
    [[nodiscard]] inline std::string name(const std::string& text) const                          \
    {                                                                                             \
        return this->name()(text);                                                                \
    }

struct Formatter
{
private:
    const uint16_t m_openingCode;
    const uint16_t m_closingCode;
    const Formatter* const m_parent;

protected:
    friend consteval Formatter CreateInstance(
        uint16_t m_openingCode,
        uint16_t m_closingCode,
        const Formatter* m_parent
    ) noexcept;

    constexpr Formatter(
        const uint16_t m_openingCode,
        const uint16_t m_closingCode,
        const Formatter* const m_parent
    ) noexcept
        :
        m_openingCode(m_openingCode),
        m_closingCode(m_closingCode),
        m_parent(m_parent)
    {
    }

public:
    MAKE_STYLE(reset, 0, 0)

    MAKE_STYLE(black, 30, 39)
    MAKE_STYLE(red, 31, 39)
    MAKE_STYLE(green, 32, 39)
    MAKE_STYLE(yellow, 33, 39)
    MAKE_STYLE(blue, 34, 39)
    MAKE_STYLE(magenta, 35, 39)
    MAKE_STYLE(cyan, 36, 39)
    MAKE_STYLE(white, 37, 39)
    MAKE_STYLE(normal, 39, 39)

    MAKE_STYLE(black_bright, 90, 39)
    MAKE_STYLE(red_bright, 91, 39)
    MAKE_STYLE(green_bright, 92, 39)
    MAKE_STYLE(yellow_bright, 93, 39)
    MAKE_STYLE(blue_bright, 94, 39)
    MAKE_STYLE(magenta_bright, 95, 39)
    MAKE_STYLE(cyan_bright, 96, 39)
    MAKE_STYLE(white_bright, 97, 39)
    MAKE_STYLE(normal_bright, 39, 39)

    MAKE_STYLE(bg_black, 40, 49)
    MAKE_STYLE(bg_red, 41, 49)
    MAKE_STYLE(bg_green, 42, 49)
    MAKE_STYLE(bg_yellow, 43, 49)
    MAKE_STYLE(bg_blue, 44, 49)
    MAKE_STYLE(bg_magenta, 45, 49)
    MAKE_STYLE(bg_cyan, 46, 49)
    MAKE_STYLE(bg_white, 47, 49)
    MAKE_STYLE(bg_normal, 49, 49)

    MAKE_STYLE(bg_black_bright, 100, 49)
    MAKE_STYLE(bg_red_bright, 101, 49)
    MAKE_STYLE(bg_green_bright, 102, 49)
    MAKE_STYLE(bg_yellow_bright, 103, 49)
    MAKE_STYLE(bg_blue_bright, 104, 49)
    MAKE_STYLE(bg_magenta_bright, 105, 49)
    MAKE_STYLE(bg_cyan_bright, 106, 49)
    MAKE_STYLE(bg_white_bright, 107, 49)
    MAKE_STYLE(bg_normal_bright, 49, 49)

    MAKE_STYLE(bold, 1, 22)
    MAKE_STYLE(faint, 2, 22)
    MAKE_STYLE(italic, 3, 23)
    MAKE_STYLE(underline, 4, 24)
    MAKE_STYLE(dbl_underline, 21, 24)
    MAKE_STYLE(blink_slow, 5, 25)
    MAKE_STYLE(blink_fast, 6, 25)
    MAKE_STYLE(invert, 7, 27)
    MAKE_STYLE(conceal, 8, 28)
    MAKE_STYLE(strikethrough, 9, 29)
    MAKE_STYLE(overline, 53, 55)

    std::string operator()(const std::string& text) const;
};

extern const Formatter FORMATTER;
