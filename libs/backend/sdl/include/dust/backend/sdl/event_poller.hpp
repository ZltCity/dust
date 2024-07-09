#pragma once

#include <dust/platform/event_poller.hpp>

namespace dust::backend::sdl
{

class EventPoller final : public dust::platform::EventPoller
{
public:
	std::optional<platform::Event> nextEvent() final;
};

} // namespace dust::backend::sdl