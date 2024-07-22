#pragma once

#include <memory>

#include <dust/gles3/buffer.hpp>
#include <dust/gles3/vertex_array.hpp>

namespace dust::scene
{

struct Mesh
{
	std::shared_ptr<gles3::Buffer> vbo, ibo;
	std::shared_ptr<gles3::VertexArray> vao;
	GLenum drawMode;
	GLsizei count;
	GLsizeiptr offset;
};

} // namespace dust::scene