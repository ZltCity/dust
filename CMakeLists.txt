cmake_minimum_required(VERSION 3.22)

project(Dust)

include(FetchContent)

option(WITH_SDL "Build with SDL2 support" ON)

if (MSVC)
	add_compile_options(/W4)
else ()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

add_subdirectory(3rdparty)
add_subdirectory(dust)
add_subdirectory(render)
