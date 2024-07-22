#pragma once

namespace dust::scene
{

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable &) = delete;
	virtual ~Drawable() noexcept = 0;

	Drawable &operator=(const Drawable &) = delete;

	virtual void present() const = 0;
};

} // namespace dust::scene