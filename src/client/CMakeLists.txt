# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_library(
    fc_client
    STATIC
    ./app.cpp
    ./inputManager.cpp
    ./render/batcher.cpp
    ./render/gl/glBatcher.cpp
    ./render/gl/glRenderer.cpp
    ./render/gl/glTexture.cpp
    ./render/renderer.cpp
    ./render/gl/shader.cpp
    ./render/texture.cpp
    ./resources.cpp
    ./scene/container.cpp
)

target_link_libraries(fc_client fc::core SDL3::SDL3)
if (NOT DEFINED EMSCRIPTEN)
    target_link_libraries(fc_client OpenGL::GL)
endif()

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
        ${FIRECAT_INCLUDE_DIR}/client/render/batcher.h
        ${FIRECAT_INCLUDE_DIR}/client/render/gl/glBatcher.h
        ${FIRECAT_INCLUDE_DIR}/client/render/gl/glRenderer.h
        ${FIRECAT_INCLUDE_DIR}/client/render/gl/glTexture.h
        ${FIRECAT_INCLUDE_DIR}/client/render/renderer.h
        ${FIRECAT_INCLUDE_DIR}/client/render/gl/shader.h
        ${FIRECAT_INCLUDE_DIR}/client/render/texture.h
        ${FIRECAT_INCLUDE_DIR}/client/resources.h
        ${FIRECAT_INCLUDE_DIR}/client/scene/container.h
)
add_library(fc::client ALIAS fc_client)
