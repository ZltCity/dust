#include <stdexcept>

#include <dust/sdl/sdl.hpp>

namespace dust::sdl
{

Lib::Lib()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw std::runtime_error(SDL_GetError());
	}
}

Lib::~Lib()
{
	SDL_Quit();
}

Lib &Lib::instance()
{
	static auto lib = Lib();

	return lib;
}

} // namespace dust::sdl