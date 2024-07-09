cmake_minimum_required(VERSION 3.22.1)

include(FetchContent)

project(DustAndroid
    C
    CXX)

set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/downloads)
set(DUST_ROOT_DIR ../../../../.. CACHE PATH "Path to Dust root dir.")
set(DUST_BUILD_ANDROID ON CACHE BOOL "Enable android build.")

add_subdirectory(${DUST_ROOT_DIR}/3rdparty ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/3rdparty)
add_subdirectory(${DUST_ROOT_DIR}/libs ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/libs)

# build native_app_glue as a static lib
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

set_target_properties(native_app_glue PROPERTIES
    C_STANDARD 11)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Dust Android
add_library(dustandroid SHARED
    src/android/event_poller.cpp
    src/android/window.cpp
    src/main.cpp)

set_target_properties(dustandroid PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON)

target_include_directories(dustandroid PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(dustandroid PRIVATE
    dust-game
    android
    native_app_glue
    log
    EGL
    GLESv3)
