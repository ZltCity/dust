#include "desktop_file_manager.hpp"
#include "desktop_file.hpp"

namespace dust::storage::desktop
{

std::unique_ptr<File> DesktopFileManager::file(const std::filesystem::path &path, StorageType storageType)
{
	return std::make_unique<DesktopFile>(path, storageType);
}

} // namespace dust::storage::desktop