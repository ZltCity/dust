#pragma once

#include <filesystem>
#include <vector>

namespace dust::util
{

[[nodiscard]] std::vector<std::byte> loadBlob(const std::filesystem::path &path);

}