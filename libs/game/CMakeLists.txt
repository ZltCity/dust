add_library(dust-game ${DUST_LIB_TYPE}
	src/game_loop.cpp)
add_library(dust::game ALIAS dust-game)

set_target_properties(dust-game PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(dust-game PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dust-game PUBLIC
	dust::render)