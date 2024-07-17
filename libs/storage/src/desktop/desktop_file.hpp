#pragma once

#include <filesystem>

#include <dust/storage/file.hpp>

namespace dust::storage::desktop
{

class DesktopFile final : public File
{
public:
	DesktopFile(std::filesystem::path filepath, StorageType storageType);

	std::unique_ptr<std::iostream> stream(StreamFlags flags) final;
	std::unique_ptr<std::istream> stream() const final;

	StorageType storageType() const final;
	bool canWrite() const final;

private:
	const std::filesystem::path m_filepath;
	const StorageType m_storageType;
};

} // namespace dust::storage::desktop