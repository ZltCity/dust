cmake_minimum_required(VERSION 3.27)

project(Dust)

include(FetchContent)

if (BUILD_SHARED_LIBS)
	set(DUST_LIB_TYPE SHARED)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
else ()
	set(DUST_LIB_TYPE STATIC)
endif ()

add_subdirectory(3rdparty)
add_subdirectory(desktop)
add_subdirectory(editor)
add_subdirectory(libs)

install(TARGETS dust-desktop dust-editor)
