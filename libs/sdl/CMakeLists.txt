add_library(dust-sdl ${DUST_LIB_TYPE}
	src/sdl.cpp
	src/window.cpp)
add_library(dust::sdl ALIAS dust-sdl)

set_target_properties(dust-sdl PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-sdl PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-sdl PUBLIC
	SDL2::SDL2
	SDL2::SDL2main)
