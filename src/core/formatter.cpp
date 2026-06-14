/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/core/formatter.h"

#include <regex>
#include <string>

const std::string CODE_START{'\x1b', '['};
const std::string CODE_REGEX_START{'\x1b', '\\', '['};

std::string Formatter::operator()(const std::string& text) const
{
    if (this == &FORMATTER)
        return text;

    const std::string opening{CODE_START + std::to_string(this->m_openingCode) + 'm'};
    const std::string closing_code = std::to_string(this->m_closingCode) + 'm';
    const std::string closing{CODE_START + closing_code};
    const std::regex closing_regex{CODE_REGEX_START + closing_code};

    const std::string body{std::regex_replace(this->m_parent->operator()(text), closing_regex, opening)};

    return opening + body + closing;
}

consteval Formatter CreateInstance(
    const unsigned short m_openingCode,
    const unsigned short m_closingCode,
    const Formatter* const m_parent
) noexcept
{
    return {m_openingCode, m_closingCode, m_parent};
}

const Formatter FORMATTER{CreateInstance(0, 0, nullptr)};
