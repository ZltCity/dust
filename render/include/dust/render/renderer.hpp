#pragma once

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

	virtual void startFrame() = 0;
	virtual void endFrame() = 0;
};

} // namespace dust::render
