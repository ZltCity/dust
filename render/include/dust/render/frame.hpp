#pragma once

namespace dust::render
{

class Frame
{
public:
	Frame() = default;
	Frame(const Frame &) = delete;
	Frame(Frame &&) noexcept = delete;

	virtual ~Frame() noexcept = 0;

	Frame &operator=(const Frame &) = delete;
	Frame &operator=(Frame &&) noexcept = delete;
};

} // namespace dust::render
