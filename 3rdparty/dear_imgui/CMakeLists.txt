FetchContent_Declare(dear_imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.90.9
	GIT_PROGRESS TRUE
	GIT_SHALLOW 1)
FetchContent_MakeAvailable(dear_imgui)

add_library(dear_imgui ${DUST_LIB_TYPE}
	${dear_imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${dear_imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
	${dear_imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
	${dear_imgui_SOURCE_DIR}/imgui.cpp
	${dear_imgui_SOURCE_DIR}/imgui_draw.cpp
	${dear_imgui_SOURCE_DIR}/imgui_tables.cpp
	${dear_imgui_SOURCE_DIR}/imgui_widgets.cpp)

add_library(dear_imgui::dear_imgui ALIAS dear_imgui)

target_link_libraries(dear_imgui PRIVATE
	freetype)
target_link_libraries(dear_imgui PUBLIC
	SDL2::SDL2)

target_include_directories(dear_imgui PUBLIC
	${dear_imgui_SOURCE_DIR})

target_compile_definitions(dear_imgui PUBLIC
	IMGUI_IMPL_OPENGL_ES3
	IMGUI_ENABLE_FREETYPE)
