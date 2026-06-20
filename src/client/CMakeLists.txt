# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_library(
    fc_client
    STATIC
    ./app.cpp
    ./inputManager.cpp
    ./render/renderer.cpp
    ./render/texture.cpp
    ./render/typeFace.cpp
    ./resources.cpp
    ./scene/container.cpp
    ./scene/sprite.cpp
    ./scene/text.cpp
)

target_link_libraries(fc_client fc::core SDL3::SDL3 skia)

target_sources(
    fc_client
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS ${FIRECAT_INCLUDE_BASE}
    FILES
        ${FIRECAT_INCLUDE_DIR}/client/app.h
        ${FIRECAT_INCLUDE_DIR}/client/color.h
        ${FIRECAT_INCLUDE_DIR}/client/inputManager.h
        ${FIRECAT_INCLUDE_DIR}/client/render/texture.h
        ${FIRECAT_INCLUDE_DIR}/client/render/typeFace.h
        ${FIRECAT_INCLUDE_DIR}/client/resources.h
        ${FIRECAT_INCLUDE_DIR}/client/scene/container.h
        ${FIRECAT_INCLUDE_DIR}/client/scene/text.h
)
add_library(fc::client ALIAS fc_client)
