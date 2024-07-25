#include <stdexcept>

#include <fmt/format.h>

#include "util.hpp"

namespace dust::editor
{

std::fstream openStream(const std::filesystem::path &path, std::ios_base::openmode mode)
{
	auto stream = std::fstream(path, mode);

	if (not stream.is_open())
	{
		throw std::runtime_error(fmt::format("Could not open file '{}'.", path.generic_string()));
	}

	return stream;
}

} // namespace dust::editor