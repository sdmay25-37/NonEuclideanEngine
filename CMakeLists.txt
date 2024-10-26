cmake_minimum_required(VERSION 3.2)

project(NonEuclideanEngine)

enable_language(ASM C CXX)
set(C_STANDARD 17)
set(C_STANDARD_REQUIRED ON)
set(C_EXTENSIONS OFF)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)
add_subdirectory(shaders)

set(ENGINE_EXECUTABLE "engine")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(${ENGINE_EXECUTABLE} ${SOURCES}
        src/engine/Engine.cpp
        src/engine/Engine.h)

target_include_directories(${ENGINE_EXECUTABLE} PUBLIC "../include/public" PRIVATE "../include/private")

target_link_libraries(${ENGINE_EXECUTABLE} glad glfw glm shaders)