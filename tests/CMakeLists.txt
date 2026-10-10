# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_library(doctest STATIC docTest.cpp)

macro(AddTestFile test_name test_file)
    add_executable(${test_name} ${test_file})
    target_link_libraries(
        ${test_name}
        PRIVATE
            doctest
            fc::core
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

addtestfile(BitStreamTest bitStream.test.cpp)

addtestfile(GridTest grid.test.cpp)

addtestfile(idPoolTest idPool.test.cpp)

addtestfile(shapeTest shapes.test.cpp)
