#include "brush.hpp"

namespace dust::rendering
{

void Brush::set() const
{
	for (const auto &[name, buffer] : ubo)
	{
		const auto blockIndex = shaderProgram->uniformBlockIndex(name);

		gles3::Binding(*buffer, blockIndex).bindBase();
	}

	shaderProgram->use();
}

} // namespace dust::rendering