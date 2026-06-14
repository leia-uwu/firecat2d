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
    if (first.getAlphaf() > 0.F) {
        ctx.canvas->drawSimpleText(
            m_text.c_str(), m_text.size(),
            SkTextEncoding::kUTF8,
            position.x, position.y,
            m_font, first
        );
    }

    if (second.getAlphaf() > 0.F) {
        ctx.canvas->drawSimpleText(
            m_text.c_str(), m_text.size(),
            SkTextEncoding::kUTF8,
            position.x, position.y,
            m_font, second
        );
    }
}
