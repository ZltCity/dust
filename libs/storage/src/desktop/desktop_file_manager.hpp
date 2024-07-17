#pragma once

#include <dust/storage/file_manager.hpp>

namespace dust::storage::desktop
{

class DesktopFileManager final : public FileManager
{
public:
	std::unique_ptr<File> file(const std::filesystem::path &path, StorageType storageType) final;
};

} // namespace dust::storage::desktop