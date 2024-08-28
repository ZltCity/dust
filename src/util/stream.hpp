#pragma once

#include <filesystem>
#include <fstream>

namespace dust::util
{

[[nodiscard]] std::fstream openStream(const std::filesystem::path &path, std::ios_base::openmode mode);

}