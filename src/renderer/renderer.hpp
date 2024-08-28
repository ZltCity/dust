#pragma once

#include <memory>

namespace dust::renderer
{

class Renderer
{
public:
	Renderer() = default;
	Renderer(const Renderer &) = delete;
	virtual ~Renderer() noexcept = 0;

	Renderer &operator=(const Renderer &) = delete;

	[[nodiscard]] static std::unique_ptr<Renderer> create(bool debug = false);
};

} // namespace dust::renderer