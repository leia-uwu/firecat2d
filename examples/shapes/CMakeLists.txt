# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_executable(shapes main.cpp)

target_link_libraries(
    shapes
    PRIVATE SDL3::SDL3
    fc::client
)

install(TARGETS shapes DESTINATION examples)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        shapes
        PROPERTIES LINK_FLAGS ${EXAMPLES_LINK_FLAGS}
    )
endif()
