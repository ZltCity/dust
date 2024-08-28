#include <algorithm>
#include <ranges>

#include <fmt/format.h>

#include "bvh.hpp"
#include "plane.hpp"

namespace dust::scene
{

class BvhBuilder
{
public:
	BvhBuilder(std::span<glm::vec3> vertices, std::span<Face> faces, int32_t leafSize = 1024)
		: m_vertices(vertices), m_faces(faces), m_leafSize(leafSize), m_faceIndices(m_faces.size())
	{
		auto rootBox = AABB();

		std::ranges::for_each(vertices, [&rootBox](const auto &v) { rootBox.extend(v); });
		std::ranges::for_each(m_faceIndices, [c = int32_t {}](auto &i) mutable { i = c++; });

		split(rootBox, m_faceIndices);
	}

	[[nodiscard]] std::vector<BVH> bvh() const
	{
		return m_bvh;
	}

private:
	[[nodiscard]] static Plane getSplitPlane(const AABB &box)
	{
		const auto boxSize = box.size();

		if (boxSize.x >= boxSize.y and boxSize.x >= boxSize.z)
		{
			return {glm::vec3(1.f, 0.f, 0.f), box.min.x + boxSize.x * .5f};
		}
		else if (boxSize.y >= boxSize.z)
		{
			return {glm::vec3(0.f, 1.f, 0.f), box.min.y + boxSize.y * .5f};
		}
		else
		{
			return {glm::vec3(0.f, 0.f, 1.f), box.min.z + boxSize.z * .5f};
		}
	}

	[[nodiscard]] int32_t split(const AABB &parentBox, std::span<int32_t> faceIndices)
	{
		if (faceIndices.size() <= static_cast<decltype(faceIndices)::size_type>(m_leafSize))
		{
			m_bvh.emplace_back(parentBox, 0);

			return 1;
		}
		else
		{
			const auto plane = getSplitPlane(parentBox);
			auto leftFaces = std::vector<int32_t> {}, rightFaces = std::vector<int32_t> {};

			for (const auto faceIndex : faceIndices)
			{
				const auto &face = m_faces[faceIndex];
				const auto faceCenter = (m_vertices[face.v1] + m_vertices[face.v2] + m_vertices[face.v3]) * (1.f / 3.f);

				if (plane.distance(faceCenter) <= 0.f)
				{
					leftFaces.push_back(faceIndex);
				}
				else
				{
					rightFaces.push_back(faceIndex);
				}
			}

			const auto leftFacesCount = static_cast<decltype(faceIndices)::difference_type>(leftFaces.size());

			std::ranges::copy(leftFaces, faceIndices.begin());
			std::ranges::copy(rightFaces, faceIndices.begin() + leftFacesCount);

			const auto [leftBox, rightBox] = parentBox.split(plane.normal, .5f);
			auto branchLength = int32_t {};

			if (not leftFaces.empty())
			{
				branchLength += split(leftBox, std::span(faceIndices.begin(), leftFaces.size()));
			}

			if (not rightFaces.empty())
			{
				branchLength += split(rightBox, std::span(faceIndices.begin() + leftFacesCount, faceIndices.end()));
			}

			m_bvh.emplace_back(parentBox, branchLength);

			return branchLength + 1;
		}
	}

	const std::span<glm::vec3> m_vertices;
	const std::span<Face> m_faces;
	const int32_t m_leafSize;

	std::vector<int32_t> m_faceIndices;
	std::vector<BVH> m_bvh;
};

std::vector<BVH> BVH::build(std::span<glm::vec3> vertices, std::span<Face> faces)
{
	return BvhBuilder(vertices, faces).bvh();
}

} // namespace dust::scene