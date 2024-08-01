#include "brush.hpp"

namespace dust::rendering
{

void Brush::set() const
{
	shaderProgram->use();
}

} // namespace dust::rendering