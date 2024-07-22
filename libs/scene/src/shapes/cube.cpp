#include <dust/scene/shapes/cube.hpp>

namespace dust::scene::shapes
{

std::vector<BasicVertex> Cube<BasicVertex>::vertices() const
{
	return {
		BasicVertex {.position = {1.f, -1.f, -1.f}}, BasicVertex {.position = {1.f, -1.f, 1.f}},
		BasicVertex {.position = {-1.f, -1.f, 1.f}}, BasicVertex {.position = {-1.f, -1.f, -1.f}},
		BasicVertex {.position = {1.f, 1.f, -0.f}},	 BasicVertex {.position = {1.f, 1.f, 1.f}},
		BasicVertex {.position = {-1.f, 1.f, 1.f}},	 BasicVertex {.position = {-1.f, 1.f, -1.f}},
	};
}

std::vector<BasicFace> Cube<BasicVertex>::faces() const
{
	return {
		BasicFace {.v1 = 2, .v2 = 3, .v3 = 4}, BasicFace {.v1 = 8, .v2 = 7, .v3 = 6},
		BasicFace {.v1 = 5, .v2 = 6, .v3 = 2}, BasicFace {.v1 = 6, .v2 = 7, .v3 = 3},
		BasicFace {.v1 = 3, .v2 = 7, .v3 = 8}, BasicFace {.v1 = 1, .v2 = 4, .v3 = 8},
		BasicFace {.v1 = 1, .v2 = 2, .v3 = 4}, BasicFace {.v1 = 5, .v2 = 8, .v3 = 6},
		BasicFace {.v1 = 1, .v2 = 5, .v3 = 2}, BasicFace {.v1 = 2, .v2 = 6, .v3 = 3},
		BasicFace {.v1 = 4, .v2 = 3, .v3 = 8}, BasicFace {.v1 = 5, .v2 = 1, .v3 = 8},
	};
}

} // namespace dust::scene::shapes