add_library(dust-scene ${DUST_LIB_TYPE}
	src/shapes/cube.cpp
	src/brush.cpp
	src/camera.cpp
	src/drawable.cpp
	src/mesh.cpp
	src/scene.cpp
	src/skybox.cpp
	src/vertex.cpp)
add_library(dust::scene ALIAS dust-scene)

set_target_properties(dust-scene PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-scene PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-scene PUBLIC
	dust::gles3
	glm::glm)