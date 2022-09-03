#pragma once

#include <memory>

namespace resources {
	class Mesh;
}

std::unique_ptr<resources::Mesh> getChunkMesh(int x, int y);
