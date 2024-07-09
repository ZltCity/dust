#pragma once

#include <stdexcept>

namespace dust::egl
{

class Error final : public std::runtime_error
{
public:
	explicit Error(const std::string &what);
};

} // namespace dust::egl