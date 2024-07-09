#pragma once

#include <functional>
#include <memory>

#include <dust/gles3/rendering_context.hpp>
#include <dust/platform/event_poller.hpp>
#include <dust/platform/window.hpp>

namespace dust::game
{

class GameLoop
{
public:
	GameLoop() = default;

	int start(std::shared_ptr<platform::EventPoller> eventPoller, std::shared_ptr<platform::Window> window);
};

} // namespace dust::game