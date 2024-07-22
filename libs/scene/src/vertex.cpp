#include <dust/scene/vertex.hpp>

namespace dust::scene
{

std::vector<gles3::VertexAttrib> BasicVertex::attributes()
{
	return {
		gles3::VertexAttrib {
			.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(BasicVertex), .offset = 0},
		gles3::VertexAttrib {
			.size = 2,
			.type = GL_FLOAT,
			.normalized = GL_FALSE,
			.stride = sizeof(BasicVertex),
			.offset = sizeof(BasicVertex::position)},
	};
}

} // namespace dust::scene