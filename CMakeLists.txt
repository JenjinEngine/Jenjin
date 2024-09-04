cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(Jenjin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Logging dependencies
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Graphics dependencies
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

find_package(Stb REQUIRED)

add_subdirectory(engine)
add_subdirectory(pong)
