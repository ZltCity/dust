#include "../gles3/bind_guard.hpp"

#include "mesh.hpp"

namespace dust::rendering
{

void Mesh::draw() const
{
	auto abind = gles3::BindGuard(*vao);
//	auto vbind = gles3::BindGuard(*vbo);
	auto ibind = gles3::BindGuard(*ibo);

	glDrawElements(mode, count, GL_UNSIGNED_INT, reinterpret_cast<const void *>(offset));
}

} // namespace dust::rendering