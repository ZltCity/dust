#include <fstream>
#include <stdexcept>

#include <fmt/format.h>

#include "desktop_file.hpp"

namespace dust::storage::desktop
{

std::unique_ptr<std::iostream> openStream(const std::filesystem::path &filepath, std::ios_base::openmode mode);

DesktopFile::DesktopFile(std::filesystem::path filepath, StorageType storageType)
	: m_filepath(std::move(filepath)), m_storageType(storageType)
{}

std::unique_ptr<std::iostream> DesktopFile::stream(StreamFlags flags)
{
	auto openMode = std::ios_base::binary | std::ios_base::in | std::ios_base::out;

	if ((flags & StreamFlags::Truncate) == StreamFlags::Truncate)
	{
		openMode |= std::ios_base::trunc;
	}

	return openStream(m_filepath, openMode);
}

std::unique_ptr<std::istream> DesktopFile::stream() const
{
	return openStream(m_filepath, std::ios_base::binary | std::ios_base::in);
}

StorageType DesktopFile::storageType() const
{
	return m_storageType;
}

bool DesktopFile::canWrite() const
{
	return m_storageType != StorageType::Asset;
}

std::unique_ptr<std::iostream> openStream(const std::filesystem::path &filepath, std::ios_base::openmode mode)
{
	auto stream = std::make_unique<std::fstream>(filepath, mode);

	if (not stream->is_open())
	{
		throw std::runtime_error(fmt::format("Could not open stream for file '{}'.", filepath.generic_string()));
	}

	return stream;
}

} // namespace dust::storage::desktop