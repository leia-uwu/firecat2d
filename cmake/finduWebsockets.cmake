# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

function(finduWebsockets)
    find_package(ZLIB QUIET)
    if (NOT ZLIB_FOUND)
        FetchContent_Declare(
            zlib
            GIT_REPOSITORY "https://github.com/madler/zlib.git"
            GIT_TAG v1.3.1
            GIT_SHALLOW ON
            GIT_SUBMODULES ""
            GIT_PROGRESS ON
        )
        FetchContent_MakeAvailable(zlib)
    endif()

    FetchContent_Declare(
        uSockets
        GIT_REPOSITORY https://github.com/uNetworking/uSockets
        GIT_TAG v0.8.8
        GIT_SHALLOW ON
        GIT_SUBMODULES ""
        GIT_PROGRESS ON
    )
    FetchContent_MakeAvailable(uSockets)

    add_library(uSockets
        ${usockets_SOURCE_DIR}/src/bsd.c
        ${usockets_SOURCE_DIR}/src/context.c
        ${usockets_SOURCE_DIR}/src/crypto
        ${usockets_SOURCE_DIR}/src/crypto/openssl.c
        ${usockets_SOURCE_DIR}/src/crypto/sni_tree.cpp
        ${usockets_SOURCE_DIR}/src/eventing
        ${usockets_SOURCE_DIR}/src/eventing/asio.cpp
        ${usockets_SOURCE_DIR}/src/eventing/epoll_kqueue.c
        ${usockets_SOURCE_DIR}/src/eventing/gcd.c
        ${usockets_SOURCE_DIR}/src/eventing/libuv.c
        ${usockets_SOURCE_DIR}/src/internal
        ${usockets_SOURCE_DIR}/src/internal/eventing
        ${usockets_SOURCE_DIR}/src/internal/eventing/asio.h
        ${usockets_SOURCE_DIR}/src/internal/eventing/epoll_kqueue.h
        ${usockets_SOURCE_DIR}/src/internal/eventing/gcd.h
        ${usockets_SOURCE_DIR}/src/internal/eventing/libuv.h
        ${usockets_SOURCE_DIR}/src/internal/internal.h
        ${usockets_SOURCE_DIR}/src/internal/loop_data.h
        ${usockets_SOURCE_DIR}/src/internal/networking
        ${usockets_SOURCE_DIR}/src/internal/networking/bsd.h
        ${usockets_SOURCE_DIR}/src/io_uring
        ${usockets_SOURCE_DIR}/src/io_uring/internal.h
        ${usockets_SOURCE_DIR}/src/io_uring/io_context.c
        ${usockets_SOURCE_DIR}/src/io_uring/io_loop.c
        ${usockets_SOURCE_DIR}/src/io_uring/io_socket.c
        ${usockets_SOURCE_DIR}/src/libusockets.h
        ${usockets_SOURCE_DIR}/src/loop.c
        ${usockets_SOURCE_DIR}/src/quic.c
        ${usockets_SOURCE_DIR}/src/quic.h
        ${usockets_SOURCE_DIR}/src/socket.c
        ${usockets_SOURCE_DIR}/src/udp.c
    )

    target_include_directories(uSockets PUBLIC ${usockets_SOURCE_DIR}/src)
    target_compile_definitions(uSockets PRIVATE LIBUS_NO_SSL)

    FetchContent_Declare(
        uWebSockets
        GIT_REPOSITORY https://github.com/uNetworking/uWebSockets
        GIT_TAG v20.78.0
        GIT_SHALLOW ON
        GIT_SUBMODULES ""
        GIT_PROGRESS ON
    )
    FetchContent_MakeAvailable(uWebSockets)

    add_library(uWebSockets INTERFACE)
    target_include_directories(uWebSockets INTERFACE ${uwebsockets_SOURCE_DIR}/src/)
    target_link_libraries(uWebSockets INTERFACE uSockets ${ZLIB_LIBRARIES})

endfunction()
