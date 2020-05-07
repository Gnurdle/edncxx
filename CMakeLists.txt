cmake_minimum_required(VERSION 3.13)
project(edncxx VERSION 0.1 LANGUAGES CXX)

include(FetchContent)
include(CTest)

option(BUILD_TESTS "Build Unit Tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)


if (BUILD_TESTS)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
    )
    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
    endif()
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    add_subdirectory(tests)
endif()
