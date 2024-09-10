file (GLOB_RECURSE SOURCES src/*.cpp)

add_executable(test ${SOURCES})
target_include_directories(test PUBLIC include)

target_link_libraries(test JenjinEngine spdlog)
