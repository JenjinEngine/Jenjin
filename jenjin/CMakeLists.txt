file (GLOB_RECURSE SOURCES src/*.cpp)

add_executable(jenjin ${SOURCES})
target_include_directories(jenjin PUBLIC include)

target_link_libraries(jenjin JenjinEngine spdlog)
