cmake_minimum_required(VERSION 3.27)

project(Dust)

include(FetchContent)

find_package(Vulkan REQUIRED COMPONENTS glslc)

add_subdirectory(3rdparty)

add_executable(dust-desktop
	src/app_info.cpp
	src/engine_info.cpp
	src/main.cpp
	src/game/config.cpp
	src/game/game.cpp
#	src/imgui/imgui.cpp
	src/logging/log.cpp
	src/renderer/vulkan/vk_context.cpp
	src/renderer/vulkan/vk_renderer.cpp
	src/renderer/renderer.cpp
	src/scene/aabb.cpp
	src/scene/bvh.cpp
	src/scene/camera.cpp
	src/scene/face.cpp
	src/scene/material.cpp
	src/scene/mesh.cpp
	src/scene/plane.cpp
#	src/scene/render_list.cpp
#	src/scene/renderer.cpp
	src/scene/scene.cpp
	src/sdl/sdl.cpp
	src/sdl/window.cpp
	src/threading/queue.cpp
	src/util/blob.cpp
	src/util/stream.cpp)

target_compile_options(dust-desktop PRIVATE
	-Wall
	-Wextra)
target_compile_definitions(dust-desktop PRIVATE
	DUST_APP_NAME="${DUST_APP_NAME}"
	DUST_APP_VERSION=${DUST_APP_VERSION})
set_target_properties(dust-desktop PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_link_libraries(dust-desktop PRIVATE
	assimp::assimp
	dear_imgui::dear_imgui
	fmt::fmt
	glm::glm
	nlohmann_json::nlohmann_json
	SDL2-static
	stb::stb
	Vulkan::Vulkan)
