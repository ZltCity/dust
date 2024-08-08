find_package(OpenGL REQUIRED)

add_executable(dust-desktop
	src/main.cpp
	src/egl/context.cpp
	src/egl/display.cpp
	src/egl/error.cpp
	src/egl/surface.cpp
	src/game/config.cpp
	src/game/game.cpp
	src/gles3/buffer.cpp
	src/gles3/rendering_context.cpp
	src/gles3/shader.cpp
	src/gles3/shader_program.cpp
	src/gles3/texture.cpp
	src/gles3/vertex_array.cpp
	src/imgui/imgui.cpp
	src/logging/log.cpp
	src/scene/aabb.cpp
	src/scene/bvh.cpp
	src/scene/camera.cpp
	src/scene/face.cpp
	src/scene/material.cpp
	src/scene/mesh.cpp
	src/scene/scene.cpp
	src/sdl/sdl.cpp
	src/sdl/window.cpp
	src/threading/queue.cpp
	src/util/blob.cpp
	src/util/stream.cpp)

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
	OpenGL::EGL
	OpenGL::GLES3)