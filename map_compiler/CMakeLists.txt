add_executable(dust-map-compiler
	src/main.cpp)

set_target_properties(dust-map-compiler PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_link_libraries(dust-map-compiler PRIVATE
	assimp::assimp
	fmt::fmt)
#target_link_libraries(dust-map-compiler PUBLIC
#        dust::threading)