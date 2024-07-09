#pragma once

namespace dust::platform
{

struct Event
{
	enum class Type
	{
		Undefined,
		Quit
	};

	Type type;
};

} // namespace dust::platform