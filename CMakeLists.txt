cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(main LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_subdirectory(monolibs)
add_subdirectory(spdlog)

add_executable(main test.cpp)
target_link_libraries(main PUBLIC monolibs spdlog)
target_include_directories(main PUBLIC monolibs spdlog)
set_target_properties(monolibs PROPERTIES LINKER_LANGUAGE CXX)
