cmake_minimum_required(VERSION 3.14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(UNIX AND NOT APPLE)
    set(VCPKG_TARGET_TRIPLET "x64-linux")
    set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
    include(${CMAKE_TOOLCHAIN_FILE})
endif()

if(WIN32)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)

    # set(GUI_TYPE WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows")
    set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)
    set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
    include(${CMAKE_TOOLCHAIN_FILE})
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(main LANGUAGES CXX VERSION 0.0.1)
message("-- Project version: ${PROJECT_VERSION}")
message("-- Build type: ${CMAKE_BUILD_TYPE}")

file(GLOB_RECURSE deps CONFIGURE_DEPENDS ./src/*.cpp ./include/*.hpp ./include/*.h)
add_executable(${PROJECT_NAME} ${GUI_TYPE} ${deps})

find_package(fmt REQUIRED)

find_package(spdlog REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    fmt::fmt
    spdlog::spdlog
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    fmt::fmt
    spdlog::spdlog
)
