# Massively Authentication

cmake_minimum_required (VERSION 3.1.0 FATAL_ERROR)

project (massively_auth CXX)

list (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

set (massively_auth_VERSION_MAJOR 0)
set (massively_auth_VERSION_MINOR 1)
set (massively_auth_VERSION_PATCH 0)
set (massively_auth_VERSION_PRERELEASE "dev")

find_package (Git)
if (GIT_FOUND)
    execute_process (
        COMMAND ${GIT_EXECUTABLE} rev-parse --verify --abbrev-ref HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE massively_auth_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process (
        COMMAND ${GIT_EXECUTABLE} rev-parse --verify --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE massively_auth_GIT_REVISION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set (massively_auth_VERSION_METADATA
        "${massively_auth_GIT_BRANCH}.${massively_auth_GIT_REVISION}"
    )
endif (GIT_FOUND)

set (massively_auth_VERSION
    "${massively_auth_VERSION_MAJOR}.${massively_auth_VERSION_MINOR}.${massively_auth_VERSION_PATCH}"
)

if (massively_auth_VERSION_PRERELEASE)
    set (massively_auth_VERSION
        "${massively_auth_VERSION}-${massively_auth_VERSION_PRERELEASE}"
    )
    if (massively_auth_VERSION_PRERELEASE STREQUAL "dev")
        set (massively_auth_VERSION
            "${massively_auth_VERSION}+${massively_auth_VERSION_METADATA}"
        )
    endif ()
endif (massively_auth_VERSION_PRERELEASE)

set (massively_auth_PACKAGE_NAME "massively_auth")
set (massively_auth_DESCRIPTION_SUMMARY "Massively Authentication")
set (massively_auth_PACKAGE_COPYRIGHT "Copyright (c) 2015, Tim Cheeseman")

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package (Boost COMPONENTS
    program_options
    REQUIRED
)

include_directories (
    "./include"
    "${PROJECT_SOURCE_DIR}/deps/spdlog/include/"
    ${Boost_INCLUDE_DIRS}
)

set (massively_auth_SRCS
    src/server.cpp
    src/options.cpp
)

add_library (massively_auth ${massively_auth_SRCS})

set_property (TARGET massively_auth PROPERTY CXX_STANDARD 14)
set_property (TARGET massively_auth PROPERTY CXX_STANDARD_REQUIRED)

target_link_libraries (massively_auth
    ${Boost_LIBRARIES}
)

set (AUTH_EXAMPLE_SRCS
    example/main.cpp
)

add_executable (auth_example ${AUTH_EXAMPLE_SRCS})

set_property (TARGET auth_example PROPERTY CXX_STANDARD 14)
set_property (TARGET auth_example PROPERTY CXX_STANDARD_REQUIRED)

target_link_libraries (auth_example
    massively_auth
)

file (COPY "example/auth.cfg" DESTINATION ".")

set (CPACK_PACKAGE_NAME ${massively_auth_PACKAGE_NAME})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${massively_auth_DESCRIPTION_SUMMARY})
set (CPACK_PACKAGE_VERSION ${massively_auth_VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR ${massively_auth_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${massively_auth_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${massively_auth_VERSION_PATCH})

set (CPACK_GENERATOR "TGZ")

include (CPack)

