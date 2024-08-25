file(GLOB SOURCES src/*.cpp)
add_executable(pong ${SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/engine/include)

target_link_libraries(pong engine)

install (TARGETS pong DESTINATION bin)
