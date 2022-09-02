#include "game/terrain.hpp"

#include "resources/mesh.hpp"

std::unique_ptr<resources::Mesh> getChunkMesh(int x, int y)
{

	std::vector<Vertex> vertices{
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
	};

	return std::make_unique<resources::Mesh>(vertices);
}
