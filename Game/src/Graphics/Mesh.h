#pragma once

#include "stdafx.h"
#include "Primitive.h"

// A mesh is a collection of primitives,
// transformations are applied to the Mesh as a whole.
typedef std::vector<Primitive> Mesh;

namespace Meshes {
	extern const Mesh CUBE;
	extern const Mesh PYRAMID;
	extern const Mesh QUAD;
}