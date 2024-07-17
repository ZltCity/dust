add_library(dust-logging ${DUST_LIB_TYPE}
	src/log.cpp)
add_library(dust::logging ALIAS dust-logging)

set_target_properties(dust-logging PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-logging PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-logging PRIVATE
	fmt::fmt)
target_link_libraries(dust-logging PUBLIC
	dust::threading)