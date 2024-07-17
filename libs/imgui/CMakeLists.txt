add_library(dust-imgui ${DUST_LIB_TYPE}
	src/imgui.cpp)
add_library(dust::imgui ALIAS dust-imgui)

set_target_properties(dust-imgui PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-imgui PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-imgui PUBLIC
	dear_imgui::dear_imgui
	dust::sdl)