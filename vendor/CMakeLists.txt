cmake_minimum_required(VERSION 3.5)

# --- GLFW ---
add_subdirectory(glfw)

# Configure GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Static build
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)

# Include it so that ImGui can use it
include_directories(glfw/include)

# --- GLAD ---
file(GLOB_RECURSE GLAD_SOURCES glad/src/*.c)
file(GLOB_RECURSE GLAD_HEADERS glad/include/*.h)

# Add the sources to the project
add_library(glad STATIC ${GLAD_SOURCES} ${GLAD_HEADERS})

# Include directories
target_include_directories(glad PUBLIC glad/include)

# --- GLM ---
add_subdirectory(glm)

# --- STB ---
file (GLOB STB_SOURCES stb/*.c)
file (GLOB STB_HEADERS stb/*.h)

add_library(stb STATIC ${STB_SOURCES} ${STB_HEADERS})

# --- SPDLOG ---
add_subdirectory(spdlog)

# --- IMGUI ---
file (GLOB IMGUI_SOURCES imgui/*.cpp)
list (APPEND IMGUI_SOURCES imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)

file (GLOB IMGUI_HEADERS imgui/*.h)
list (APPEND IMGUI_HEADERS imgui/backends/imgui_impl_glfw.h imgui/backends/imgui_impl_opengl3.h)

include_directories(imgui)
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# --- Lua ---
file (GLOB LUA_SOURCES lua/*.c)
file (GLOB LUA_HEADERS lua/*.h)

include_directories(lua)

add_library(lua STATIC ${LUA_SOURCES} ${LUA_HEADERS})

# --- Sol2 ---
include_directories(sol2)
add_subdirectory(sol2)
