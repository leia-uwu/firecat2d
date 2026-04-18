# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_library(
    fc_core
    STATIC
    ./bitStream.cpp
    ./collision/collision.cpp
    ./collision/shape.cpp
    ./formatter.cpp
    ./ticker.cpp
)

target_sources(
    fc_core
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS ${FIRECAT_INCLUDE_BASE}
    FILES
        ${FIRECAT_INCLUDE_DIR}/core/bitStream.h
        ${FIRECAT_INCLUDE_DIR}/core/buffer.h
        ${FIRECAT_INCLUDE_DIR}/core/collision/collision.h
        ${FIRECAT_INCLUDE_DIR}/core/collision/grid.h
        ${FIRECAT_INCLUDE_DIR}/core/collision/shape.h
        ${FIRECAT_INCLUDE_DIR}/core/formatter.h
        ${FIRECAT_INCLUDE_DIR}/core/idPool.h
        ${FIRECAT_INCLUDE_DIR}/core/math/gmath.h
        ${FIRECAT_INCLUDE_DIR}/core/math/vec2.h
        ${FIRECAT_INCLUDE_DIR}/core/ticker.h
)
add_library(fc::core ALIAS fc_core)
