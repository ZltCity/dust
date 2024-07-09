#include <SDL.h>

#include <dust/backend/sdl/event_poller.hpp>

namespace dust::backend::sdl
{

[[nodiscard]] platform::Event mapEvent(const SDL_Event &event);

std::optional<platform::Event> EventPoller::nextEvent()
{
	auto event = SDL_Event {};

	if (not SDL_PollEvent(&event))
	{
		return {};
	}

	return mapEvent(event);
}

platform::Event mapEvent(const SDL_Event &event)
{
	switch (event.type)
	{
		case SDL_QUIT: return {.type = platform::Event::Type::Quit};

		default: return {};
	}
}

} // namespace dust::backend::sdl
