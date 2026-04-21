# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_library(doctest SHARED docTest.cpp)

macro(AddTestFile test_name test_file)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PRIVATE doctest fc::core)

    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endmacro()

AddTestFile(BitStreamTest bitStream.test.cpp)

AddTestFile(GridTest grid.test.cpp)

AddTestFile(idPoolTest idPool.test.cpp)

AddTestFile(shapeTest shapes.test.cpp)
