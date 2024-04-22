#pragma once

namespace dust::render
{

class Frame
{
public:
	Frame() = default;
	Frame(const Frame &) = delete;
	virtual ~Frame() noexcept = 0;

	Frame &operator=(const Frame &) = delete;
};

} // namespace dust::render
