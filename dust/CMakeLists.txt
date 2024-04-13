add_executable(dust
	src/main.cpp)

set_target_properties(dust PROPERTIES
	CXX_STANDARD 20)

target_link_libraries(dust PRIVATE
	dust::render)