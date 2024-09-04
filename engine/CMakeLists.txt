file (GLOB_RECURSE SRC_FILES src/*.cpp)
add_library(JenjinEngine ${SRC_FILES})

target_include_directories(JenjinEngine
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
				${Stb_INCLUDE_DIR}
)

target_link_libraries(JenjinEngine PRIVATE spdlog::spdlog glfw glad::glad glm::glm imgui::imgui fmt::fmt)
