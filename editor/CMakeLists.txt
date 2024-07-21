add_executable(dust-editor
	src/config.cpp
	src/main.cpp)

set_target_properties(dust-editor PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_link_libraries(dust-editor PRIVATE
	dust::gles3
	dust::imgui
	dust::logging
	dust::scene
	dust::storage
	fmt::fmt
	glm::glm
	nlohmann_json::nlohmann_json)