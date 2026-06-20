/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#pragma once

#include "fc/client/render/typeFace.h"
#include "fc/client/scene/container.h"

#include <cstdint>
#include <include/core/SkCanvas.h>
#include <include/core/SkColorFilter.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkTypeface.h>

#include <utility>

class TextItem : public Container
{
public:
    TextItem() : Container()
    {
        m_strokePaint.setStyle(SkPaint::kStroke_Style);
        m_strokePaint.setAlphaf(0);
    }

    TextItem(std::string text) : Container(), m_text(std::move(text))
    {
        m_strokePaint.setStyle(SkPaint::kStroke_Style);
        m_strokePaint.setAlphaf(0);
    }

    void setText(const std::string& text)
    {
        m_text = text;
    }

    TypeFace* typeFace()
    {
        return m_typeFace;
    }

    void setTypeFace(TypeFace* typeFace)
    {
        m_typeFace = typeFace;
    }

    [[nodiscard]] SkFont& font()
    {
        return m_font;
    }

    void setFont(const SkFont& font)
    {
        m_font = font;
    }

    enum PaintOrder : uint8_t {
        STROKE_FILL,
        FILL_STROKE
    };

    void setPaintOrder(PaintOrder order)
    {
        m_paintOrder = order;
    }

    [[nodiscard]] PaintOrder paintOrder() const
    {
        return m_paintOrder;
    }

    SkPaint& fillPaint()
    {
        return m_fillPaint;
    }

    void setFillPaint(const SkPaint& paint)
    {
        m_fillPaint = paint;
    }

    SkPaint& strokePaint()
    {
        return m_strokePaint;
    }

    void setStrokePaint(const SkPaint& paint)
    {
        m_strokePaint = paint;
    }

    float getWidth();

    void render(Renderer::RenderCtx& ctx) override;

private:
    TypeFace* m_typeFace;
    SkFont m_font;

    PaintOrder m_paintOrder = STROKE_FILL;

    SkPaint m_fillPaint;
    SkPaint m_strokePaint;

    std::string m_text;
};
