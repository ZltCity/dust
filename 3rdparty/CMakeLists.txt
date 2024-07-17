add_subdirectory(dear_imgui)
add_subdirectory(format)
add_subdirectory(freetype)
add_subdirectory(glm)
add_subdirectory(json)

if (NOT DUST_BUILD_ANDROID)
	add_subdirectory(sdl)
endif ()

add_subdirectory(stb_image)
