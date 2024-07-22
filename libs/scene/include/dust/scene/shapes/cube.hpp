#pragma once

#include <vector>

#include <dust/scene/face.hpp>
#include <dust/scene/vertex.hpp>

namespace dust::scene::shapes
{

template<class TVertex>
struct Cube
{};

template<>
struct Cube<BasicVertex>
{
	[[nodiscard]] std::vector<BasicVertex> vertices() const;
	[[nodiscard]] std::vector<BasicFace> faces() const;
};

} // namespace dust::scene::shapes