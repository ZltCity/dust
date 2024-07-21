#include <GLES3/gl32.h>

#include <dust/game/game_loop.hpp>
#include <dust/gles3/rendering_context.hpp>

namespace dust::game
{

int GameLoop::start(std::shared_ptr<sdl::Window> window)
{
	auto renderingContext = gles3::RenderingContext(window);
	auto quit = false;

	while (not quit)
	{
		auto event = SDL_Event {};

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		renderingContext.makeCurrent();
		glClearColor(0.5f, 0.6f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		renderingContext.swapBuffers();
	}

	return 0;
}

} // namespace dust::game