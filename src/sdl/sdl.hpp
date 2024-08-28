#pragma once

#include <SDL.h>

namespace dust::sdl
{

class Lib
{
public:
	Lib(const Lib &) = delete;
	~Lib();

	Lib &operator=(const Lib &) = delete;

	[[nodiscard]] static Lib &instance();

private:
	Lib();
};

} // namespace dust::sdl