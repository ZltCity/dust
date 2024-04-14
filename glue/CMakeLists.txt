add_library(glue STATIC
	src/glue.cpp)

add_library(dust::glue ALIAS glue)

set_target_properties(glue PROPERTIES
	CXX_STANDARD 20)

target_include_directories(glue PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)
