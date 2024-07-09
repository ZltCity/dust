find_package(OpenGL REQUIRED)

add_library(dust-render ${DUST_LIB_TYPE}
	src/egl/context.cpp
	src/egl/display.cpp
	src/egl/error.cpp
	src/egl/surface.cpp
	src/rendering_context.cpp)
add_library(dust::render ALIAS dust-render)

set_target_properties(dust-render PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-render PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-render PRIVATE
	fmt::fmt)
target_link_libraries(dust-render PUBLIC
	dust::platform
	OpenGL::EGL
	OpenGL::GLES3)