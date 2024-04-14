#include <iostream>

#define SDL_MAIN_HANDLED
#include <dust/render/renderer.hpp>

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	auto window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> {
		SDL_CreateWindow("Test", 50, 50, 1280, 720, SDL_WINDOW_VULKAN), SDL_DestroyWindow};
	const auto appInfo = dust::glue::ApplicationInfo {.applicationName = "Dust", .applicationVersion = 0};
	auto renderer = dust::render::createRenderer(appInfo, window.get());
	auto quit = false;

	while (not quit)
	{
		auto event = SDL_Event {};

		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					quit = true;
					break;
				}
			}
		}
	}

	return 0;
}
