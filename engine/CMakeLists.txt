cmake_minimum_required(VERSION 3.5)

file (GLOB_RECURSE SOURCES src/*.cpp)
add_library (JenjinEngine ${SOURCES})

target_include_directories(JenjinEngine
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
				${CMAKE_SOURCE_DIR}/vendor/glfw/include
				${CMAKE_SOURCE_DIR}/vendor/glad/include
				${CMAKE_SOURCE_DIR}/vendor/glm/glm/
				${CMAKE_SOURCE_DIR}/vendor/imgui
				${CMAKE_SOURCE_DIR}/vendor/imgui/backends
				${CMAKE_SOURCE_DIR}/vendor/stb
				${CMAKE_SOURCE_DIR}/vendor/lua
				${CMAKE_SOURCE_DIR}/vendor/sol2/include
				${CMAKE_SOURCE_DIR}/vendor/
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(JenjinEngine PRIVATE glfw glad spdlog glm stb imgui lua sol2)
