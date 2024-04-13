#include <iostream>

#define SDL_MAIN_HANDLED
#include <dust/render/renderer.hpp>

int main(int argc, char **argv)
{
	auto backends = dust::render::getSupportedBackends();

	for (auto &back : backends)
	{
		switch (back)
		{
			case dust::render::Backend::Vulkan: std::cout << "Vulkan is supported!" << std::endl; break;
		}
	}

	SDL_Init(SDL_INIT_EVERYTHING);

	auto window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> {
		SDL_CreateWindow("Test", 50, 50, 1280, 720, SDL_WINDOW_VULKAN), SDL_DestroyWindow};
	auto renderer = dust::render::createRenderer(window.get());
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
