cmake_minimum_required(VERSION 3.27)

project(Dust)

include(FetchContent)

if (BUILD_SHARED_LIBS)
	set(DUST_LIB_TYPE SHARED)
else ()
	set(DUST_LIB_TYPE STATIC)
endif ()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(3rdparty)
add_subdirectory(desktop)
add_subdirectory(editor)
add_subdirectory(libs)
add_subdirectory(map_compiler)

#install(TARGETS dust-desktop dust-editor)
