file (GLOB ENGINE_SOURCES src/*.cpp)
add_library(engine STATIC ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC include)

install(TARGETS engine
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
