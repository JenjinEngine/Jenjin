cmake_minimum_required(VERSION 3.5)

file (GLOB_RECURSE SOURCES src/*.cpp)
add_library (JenjinEngine ${SOURCES})

target_include_directories(JenjinEngine
		PUBLIC
				$<INSTALL_INTERFACE:include>
				$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
				${CMAKE_SOURCE_DIR}/megasource/libs/glfw/include/
				${CMAKE_SOURCE_DIR}/megasource/libs/glad/include/
				${CMAKE_SOURCE_DIR}/megasource/libs/glm/
				${CMAKE_SOURCE_DIR}/megasource/libs/imgui/
				${CMAKE_SOURCE_DIR}/megasource/libs/imgui/backends/
				${CMAKE_SOURCE_DIR}/megasource/libs/stb/
				${CMAKE_SOURCE_DIR}/megasource/libs/lua/include/
				${CMAKE_SOURCE_DIR}/megasource/libs/sol2/include/
				${CMAKE_SOURCE_DIR}/megasource/libs/IconFontCppHeaders/
				${CMAKE_SOURCE_DIR}/megasource/libs/
		PRIVATE
				${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(JenjinEngine PRIVATE glfw glad spdlog glm stb imgui lua54 sol2)
