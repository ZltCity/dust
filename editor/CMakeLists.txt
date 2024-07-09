add_executable(dust-editor
	src/main.cpp)

set_target_properties(dust-editor PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_link_libraries(dust-editor PRIVATE
	SDL2::SDL2
	SDL2::SDL2main)