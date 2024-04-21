#pragma once

#include "frame.hpp"

namespace dust::render
{

class Renderer
{
public:
	Renderer() = default;
	Renderer(const Renderer &) = delete;
	Renderer(Renderer &&) noexcept = delete;

	virtual ~Renderer() noexcept = 0;

	Renderer &operator=(const Renderer &) = delete;
	Renderer &operator=(Renderer &&) noexcept = delete;

	virtual std::shared_ptr<Frame> createFrame() = 0;
};

} // namespace dust::render
