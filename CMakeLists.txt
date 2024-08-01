cmake_minimum_required(VERSION 3.27)

project(Dust)

include(FetchContent)

#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(3rdparty)
add_subdirectory(desktop)

#install(TARGETS dust-desktop dust-editor)
