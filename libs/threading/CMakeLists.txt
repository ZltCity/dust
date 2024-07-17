add_library(dust-threading ${DUST_LIB_TYPE}
	src/queue.cpp)
add_library(dust::threading ALIAS dust-threading)

set_target_properties(dust-threading PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-threading PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-threading PUBLIC
	atomic)