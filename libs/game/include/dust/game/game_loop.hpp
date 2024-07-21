#pragma once

#include <functional>
#include <memory>

#include <dust/gles3/rendering_context.hpp>
#include <dust/sdl/window.hpp>

namespace dust::game
{

class GameLoop
{
public:
	GameLoop() = default;

	int start(std::shared_ptr<sdl::Window> window);
};

} // namespace dust::game