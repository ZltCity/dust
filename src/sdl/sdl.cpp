#include <stdexcept>

#include <SDL_vulkan.h>

#include "sdl.hpp"

namespace dust::sdl
{

Lib::Lib()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw std::runtime_error(SDL_GetError());
	}

	if (SDL_Vulkan_LoadLibrary(nullptr) != 0)
	{
		throw std::runtime_error(SDL_GetError());
	}
}

Lib::~Lib()
{
	SDL_Vulkan_UnloadLibrary();
	SDL_Quit();
}

Lib &Lib::instance()
{
	static auto lib = Lib();

	return lib;
}

} // namespace dust::sdl