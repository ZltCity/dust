#pragma once

#include <filesystem>
#include <fstream>

namespace dust::editor
{

std::fstream openStream(const std::filesystem::path &path, std::ios_base::openmode mode);

}