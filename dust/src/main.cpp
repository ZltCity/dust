#include <iostream>

#include <fmt/format.h>

#define SDL_MAIN_HANDLED
#include <dust/render/backend.hpp>

int main(int argc, char **argv)
try
{
	SDL_Init(SDL_INIT_EVERYTHING);

	auto window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> {
		SDL_CreateWindow("Test", 50, 50, 1280, 720, SDL_WINDOW_VULKAN), SDL_DestroyWindow};
	const auto appInfo = dust::glue::ApplicationInfo {.applicationName = "Dust", .applicationVersion = 0};
	auto backend = dust::render::createBackend(dust::render::BackendName::Vulkan, appInfo, window.get());

	std::cout << "Found suitable GPUs:\n";

	for (const auto &device : backend->getSuitableDevices())
	{
		std::cout << fmt::format("{}: {}\n", device.systemIndex, device.name);
	}

	std::cout << std::endl;

	auto renderer = backend->createRenderer({dust::render::Hint {dust::render::HintName::UseDevice, 0}});
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
catch (const std::exception &ex)
{
	std::cout << ex.what() << std::endl;
	std::exit(-1);
}
