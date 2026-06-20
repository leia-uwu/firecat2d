/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/typeFace.h"
#include "fc/client/resources.h"
#include "fc/core/buffer.h"
#include <include/core/SkData.h>

void TypeFace::load(const Uint8Buffer& data)
{
    assert(m_loaded == false);

    m_loaded = true;
    m_typeFace = Resources::get().fontManager()->makeFromData(
        SkData::MakeWithCopy(data.data(), data.size())
    );
}
