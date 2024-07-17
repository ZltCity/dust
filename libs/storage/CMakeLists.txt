if (${CMAKE_SYSTEM_NAME} STREQUAL Android)
else ()
	set(SOURCES
		src/desktop/desktop_file.cpp
		src/desktop/desktop_file_manager.cpp)
endif ()

add_library(dust-storage ${DUST_LIB_TYPE}
	src/file.cpp
	src/file_manager.cpp
	${SOURCES})
add_library(dust::storage ALIAS dust-storage)

set_target_properties(dust-storage PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-storage PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-storage PRIVATE
	fmt::fmt)
target_link_libraries(dust-storage PUBLIC
	dust::concepts)
