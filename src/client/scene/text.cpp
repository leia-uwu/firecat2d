/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/scene/text.h"

#include "include/core/SkTypeface.h"

void TextItem::render(Renderer::RenderCtx& ctx)
{
    SkPaint& first = m_paintOrder == STROKE_FILL ? m_strokePaint : m_fillPaint;
    SkPaint& second = m_paintOrder == STROKE_FILL ? m_fillPaint : m_strokePaint;

    if (m_font.getTypeface() != m_typeFace->typeFace().get()) {
        m_font.setTypeface(m_typeFace->typeFace());
    }

    if (first.getAlphaf() > 0.F) {
        ctx.canvas->drawSimpleText(
            m_text.c_str(), m_text.size(),
            SkTextEncoding::kUTF8,
            0, 0,
            m_font, first
        );
    }

    if (second.getAlphaf() > 0.F) {
        ctx.canvas->drawSimpleText(
            m_text.c_str(), m_text.size(),
            SkTextEncoding::kUTF8,
            0, 0,
            m_font, second
        );
    }
}

float TextItem::getWidth()
{
    float width = 0;

    if (m_fillPaint.getAlphaf() > 0.F) {
        width = m_font.measureText(
            m_text.data(), m_text.size(),
            SkTextEncoding::kUTF8,
            nullptr,
            &m_fillPaint
        );
    }

    if (m_strokePaint.getAlphaf() > 0.F) {
        float strokeWidth = m_font.measureText(
            m_text.data(), m_text.size(),
            SkTextEncoding::kUTF8,
            nullptr,
            &m_strokePaint
        );
        width = std::max(width, strokeWidth);
    }

    return width;
}
