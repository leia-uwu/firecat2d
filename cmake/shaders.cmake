# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

find_program(GLSLANG glslang)

function(compileShader path name type)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/shaders)

    set(shaderIn ${CMAKE_CURRENT_SOURCE_DIR}/${path}/${name}.${type})

    set(shaderOut ${CMAKE_BINARY_DIR}/include/shaders/${name}.${type})
    set(shaderOutSpv ${shaderOut}.spv)
    set(shaderHeaderOut ${shaderOut}.h)

    set(${name}_${type}_out ${shaderHeaderOut} PARENT_SCOPE)

    # if (NOT APPLE)
        add_custom_command(
            COMMAND ${GLSLANG} -V -o ${shaderHeaderOut} ${shaderIn} --vn ${name}_${type}
            OUTPUT ${shaderHeaderOut}
            DEPENDS ${shaderIn}
            VERBATIM
        )
    # else()
    #     find_program(SPIRV_CROSS spirv-cross)
    #     add_custom_command(
    #         COMMAND ${GLSLANG} -V -o ${shaderOut} ${shaderIn}
    #         OUTPUT ${shaderOut}
    #         DEPENDS ${shaderIn}
    #         VERBATIM
    #     )
    #     add_custom_command(
    #         COMMAND ${SPIRV_CROSS} --msl ${shaderOut} --output ${shaderOut}.msl
    #         OUTPUT ${shaderOut}.msl
    #         DEPENDS ${shaderOut}
    #         VERBATIM
    #     )
    #     add_custom_command(
    #         COMMAND sh -c "echo -e \"#pragma once\\nconst int ${name}_${type}[] = {\\n    #embed <${shaderOut}.msl>\\n};\\n\" > ${shaderHeaderOut}"
    #         OUTPUT ${shaderHeaderOut}
    #         DEPENDS ${shaderOut}.msl
    #         VERBATIM
    #     )
    # endif()

endfunction()
