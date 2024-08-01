#pragma once

#include <memory>

#include "../gles3/buffer.hpp"
#include "../gles3/vertex_array.hpp"

namespace dust::rendering
{

struct Mesh
{
	void draw() const;

	std::shared_ptr<gles3::Buffer> vbo, ibo;
	std::shared_ptr<gles3::VertexArray> vao;
	GLenum mode;
	GLsizei count;
	GLsizei offset = 0;
};

} // namespace dust::rendering