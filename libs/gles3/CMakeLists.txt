find_package(OpenGL REQUIRED)

add_library(dust-gles3 ${DUST_LIB_TYPE}
	src/egl/context.cpp
	src/egl/display.cpp
	src/egl/error.cpp
	src/egl/surface.cpp
	src/rendering_context.cpp)
add_library(dust::gles3 ALIAS dust-gles3)

set_target_properties(dust-gles3 PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-gles3 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-gles3 PRIVATE
	fmt::fmt)
target_link_libraries(dust-gles3 PUBLIC
	dust::sdl
	OpenGL::EGL
	OpenGL::GLES3)
