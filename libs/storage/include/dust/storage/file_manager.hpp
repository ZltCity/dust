#pragma once

#include <filesystem>

#include <dust/storage/file.hpp>
#include <dust/storage/storage_type.hpp>

namespace dust::storage
{

class FileManager
{
public:
	FileManager() = default;
	FileManager(const FileManager &) = delete;
	virtual ~FileManager() noexcept = 0;

	FileManager &operator=(const FileManager &) = delete;

	[[nodiscard]] virtual std::unique_ptr<File> file(const std::filesystem::path &path, StorageType storageType) = 0;

	[[nodiscard]] static std::unique_ptr<FileManager> create();
};

} // namespace dust::storage
