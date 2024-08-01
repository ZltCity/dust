#include "blob.hpp"
#include "stream.hpp"

namespace dust::util
{

std::vector<std::byte> loadBlob(const std::filesystem::path &path)
{
	auto stream = openStream(path, std::ios_base::binary | std::ios_base::in);
	auto blob = std::vector<std::byte>(std::filesystem::file_size(path));

	stream.read(reinterpret_cast<char *>(blob.data()), static_cast<std::streamsize>(blob.size()));

	return blob;
}

} // namespace dust::util