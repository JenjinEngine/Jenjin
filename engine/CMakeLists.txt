include(FetchContent)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)

# Graphics library
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
FetchContent_MakeAvailable(glfw)

# Logging library
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog GIT_TAG v1.x)
FetchContent_MakeAvailable(spdlog)

# GLAD library
FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG master)
FetchContent_GetProperties(glad)
set(GLAD_PROFILE "core" CACHE STRING "" FORCE)
set(GLAD_API "gl=3.3" CACHE STRING "" FORCE)
set(GLAD_GENERATOR "c" CACHE STRING "" FORCE)
FetchContent_MakeAvailable(glad)

# ImGui library
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG docking)
FetchContent_GetProperties(imgui)

if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR}/)
    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp)
    file(GLOB IMGUI_HEADERS ${imgui_SOURCE_DIR}/*.h)
    add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_SOURCES} ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
    target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR})
    target_link_libraries(imgui ${OPENGL_LIBRARIES} glfw)
endif ()

# STB library
FetchContent_Declare(stb GIT_REPOSITORY https://github.com/nothings/stb.git GIT_TAG master)
FetchContent_MakeAvailable(stb)

# GLM library for math
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG master)
FetchContent_MakeAvailable(glm)

# Build options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_STANDARD 20)

# Configure GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file (GLOB ENGINE_SOURCES src/*.cpp)
add_library(engine STATIC ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC include ${IMGUI_INCLUDE_DIR} ${stb_SOURCE_DIR} ${glm_SOURCE_DIR})
target_link_libraries(engine glfw spdlog OpenGL::GL glad imgui)

install(TARGETS engine
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
