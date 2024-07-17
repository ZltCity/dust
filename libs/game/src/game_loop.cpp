#include <GLES3/gl32.h>

#include <dust/game/game_loop.hpp>

namespace dust::game
{

int GameLoop::start(std::shared_ptr<platform::EventPoller> eventPoller, std::shared_ptr<platform::Window> window)
{
	auto renderingContext = gles3::RenderingContext(window);
	auto quit = false;

	while (not quit)
	{
		auto event = std::optional<platform::Event>(std::nullopt);

		while ((event = eventPoller->nextEvent()).has_value())
		{
			if (event->type == platform::Event::Type::Quit)
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