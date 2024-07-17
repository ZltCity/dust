add_library(dust-concepts INTERFACE)
add_library(dust::concepts ALIAS dust-concepts)

set_target_properties(dust-concepts PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-concepts INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include)
