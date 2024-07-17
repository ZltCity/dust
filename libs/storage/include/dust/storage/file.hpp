#pragma once

#include <iostream>
#include <memory>

#include <dust/storage/storage_type.hpp>
#include <dust/storage/stream_flags.hpp>

namespace dust::storage
{

class File
{
public:
	File() = default;
	File(const File &) = delete;
	virtual ~File() noexcept = 0;

	File &operator=(const File &) = delete;

	[[nodiscard]] virtual std::unique_ptr<std::iostream> stream(StreamFlags flags) = 0;
	[[nodiscard]] virtual std::unique_ptr<std::istream> stream() const = 0;

	[[nodiscard]] virtual StorageType storageType() const = 0;
	[[nodiscard]] virtual bool canWrite() const = 0;
};

} // namespace dust::storage
