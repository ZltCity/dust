add_executable(dust-desktop
	src/main.cpp)

set_target_properties(dust-desktop PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_link_libraries(dust-desktop PRIVATE
	dust::game
	dust::sdl)