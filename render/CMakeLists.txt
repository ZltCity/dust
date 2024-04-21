find_package(Vulkan REQUIRED)

add_library(render STATIC
	src/vulkan/vulkan_backend.cpp
	src/vulkan/vulkan_renderer.cpp
	src/vulkan/vulkan_util.cpp
	src/backend.cpp
	src/renderer.cpp)

add_library(dust::render ALIAS render)

set_target_properties(render PROPERTIES
	CXX_STANDARD 20)

if (${WITH_SDL})
	target_compile_definitions(render PUBLIC WITH_SDL)
	target_link_libraries(render PUBLIC
		SDL2::SDL2)
endif ()

target_link_libraries(render PRIVATE
	fmt::fmt)

target_link_libraries(render PUBLIC
	dust::glue
	Vulkan::Vulkan)

target_include_directories(render PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)
