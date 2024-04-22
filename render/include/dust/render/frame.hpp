#pragma once

#include <array>

namespace dust::render
{

class Frame
{
public:
	Frame() = default;
	Frame(const Frame &) = delete;
	virtual ~Frame() noexcept = 0;

	Frame &operator=(const Frame &) = delete;

	virtual void clearColor(const std::array<float, 4> &value) = 0;
	virtual void present() = 0;
};

} // namespace dust::render
