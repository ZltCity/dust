add_library(render STATIC
	src/backend.cpp
	src/render.cpp)

add_library(dust::render ALIAS render)

set_target_properties(render PROPERTIES
	CXX_STANDARD 20)

target_include_directories(render PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)
