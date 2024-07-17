#if defined(__ANDROID__)
#else
#include "desktop/desktop_file_manager.hpp"
#endif

namespace dust::storage
{

FileManager::~FileManager() noexcept = default;

std::unique_ptr<FileManager> FileManager::create()
{
#if defined(__ANDROID__)
	return {};
#else
	return std::make_unique<desktop::DesktopFileManager>();
#endif
}
} // namespace dust::storage