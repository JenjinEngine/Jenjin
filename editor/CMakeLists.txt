file (GLOB_RECURSE SOURCES src/*.cpp)

add_executable(editor ${SOURCES})
target_include_directories(editor PUBLIC include)

target_link_libraries(editor JenjinEngine spdlog)
