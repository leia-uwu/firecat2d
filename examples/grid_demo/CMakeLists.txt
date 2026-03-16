# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_executable(grid_demo main.cpp)

target_link_libraries(
    grid_demo
    fc::client
)

install(TARGETS grid_demo DESTINATION examples)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        grid_demo
        PROPERTIES LINK_FLAGS ${EXAMPLES_LINK_FLAGS}
    )
endif()
