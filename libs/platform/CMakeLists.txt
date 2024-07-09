add_library(dust-platform ${DUST_LIB_TYPE}
	src/event_poller.cpp
	src/window.cpp)
add_library(dust::platform ALIAS dust-platform)

set_target_properties(dust-platform PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-platform PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)
