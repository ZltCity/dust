#include <dust/render/backend.hpp>

namespace dust::render
{

std::vector<Backend> getSupportedBackends()
{
	return {Backend::Vulkan};
}

} // namespace dust::render
